# device_comm.py
import serial
from serial.tools import list_ports

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

    def receive(self) -> bytes:
        if self.is_connected():
            return self.serial.read(1024)
        return b''