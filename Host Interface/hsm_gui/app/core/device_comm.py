# app/core/device_comm.py

import serial
from serial.tools import list_ports
from app.core.logger import log
import time

class DeviceInterface:
    def __init__(self, baudrate = 115200):
        self.baudrate = baudrate
        self.serial = None

    def auto_detect_port(self):
        TARGET_VID = 0x0483
        TARGET_PID = 0x5740

        for port in list_ports.comports():
            if port.vid == TARGET_VID and port.pid == TARGET_PID:
                return port.device
        return None

    def connect(self):
        port = self.auto_detect_port()
        if not port:
            raise Exception("Device not found. Please check connection.")
        try:
            self.serial = serial.Serial(port, self.baudrate, timeout=1)
            return True
        except serial.SerialException as e:
            raise Exception(f"Failed to connect to {port}: {e}")
    
    def disconnect(self):
        if self.serial and self.serial.is_open:
            self.serial.close()
            return True
        return False
    
    def is_connected(self):
        return self.serial and self.serial.is_open
    
    def send(self, data: bytes):
        if self.is_connected():
            self.serial.write(data)
            log(f"[TX] {data.hex(' ').upper()}")

    def receive(self) -> bytes:
        if not self.is_connected():
            return b''
        
        buffer = bytearray()
        timeout = 5 # seconds
        end_marker = b'\xCA\xFE\xBA\BE'

        start_time = time.time()
        while True:
            if self.serial.in_waiting:
                byte = self.serial.read(1)
                buffer += byte
                if buffer[-4:] == end_marker:
                    log(f"[RX] {buffer.hex(' ').upper()}")
                    return bytes(buffer)
        
            if time.time() - start_time > timeout:
                log("[RX] Timeout waiting for complete packet")
                raise TimeoutError("Timeout waiting for response from device.")