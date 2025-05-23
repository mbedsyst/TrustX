import sys
import serial
import random
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QPushButton, QTextEdit,
    QHBoxLayout, QStackedWidget, QListWidget, QListWidgetItem, QComboBox,
    QLabel, QFileDialog
)
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QColor, QTextCharFormat


class DevicePage(QWidget):
    def __init__(self, serial_port_callback):
        super().__init__()
        self.serial_port_callback = serial_port_callback
        layout = QVBoxLayout()

        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(self.connect_serial)
        self.connect_button.setStyleSheet("background-color: navy; color: white; padding: 5px;")

        self.disconnect_button = QPushButton("Disconnect")
        self.disconnect_button.clicked.connect(self.disconnect_serial)
        self.disconnect_button.setStyleSheet("background-color: navy; color: white; padding: 5px;")

        btn_layout = QHBoxLayout()
        btn_layout.addWidget(self.connect_button)
        btn_layout.addWidget(self.disconnect_button)

        layout.addLayout(btn_layout)
        self.setLayout(layout)

    def connect_serial(self):
        self.serial_port_callback("connect")

    def disconnect_serial(self):
        self.serial_port_callback("disconnect")


class RNGPage(QWidget):
    def __init__(self, send_packet_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        layout = QVBoxLayout()

        self.dropdown = QComboBox()
        self.dropdown.addItems(["4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096"])
        layout.addWidget(QLabel("Select Byte Size for RNG:"))
        layout.addWidget(self.dropdown)

        self.execute_button = QPushButton("Execute")
        self.execute_button.clicked.connect(self.send_rng_packet)
        layout.addWidget(self.execute_button)

        self.setLayout(layout)

    def send_rng_packet(self):
        size_to_option = {
            "4": 0x31, "8": 0x32, "16": 0x33, "32": 0x34, "64": 0x35, "128": 0x36,
            "256": 0x37, "512": 0x38, "1024": 0x39, "2048": 0x3A, "4096": 0x3B
        }
        option = size_to_option[self.dropdown.currentText()]
        txid = random.getrandbits(32).to_bytes(4, 'big')
        cmd = bytes([0x04])
        option_byte = bytes([option])
        input_size = b"\x00\x00\x00"
        eod = bytes.fromhex("DEADBEEF")
        packet = txid + cmd + option_byte + input_size + eod
        self.send_packet_callback(packet)


class HashingPage(QWidget):
    def __init__(self, send_packet_callback, log_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        self.log_callback = log_callback

        self.max_input_size = int(65536 * 0.95)

        layout = QVBoxLayout()

        self.dropdown = QComboBox()
        self.dropdown.addItems([
            "SHA224", "SHA256", "SHA384", "SHA512"
        ])
        layout.addWidget(QLabel("Select Hashing Algorithm:"))
        layout.addWidget(self.dropdown)

        self.input_field = QTextEdit()
        layout.addWidget(self.input_field)

        self.attach_button = QPushButton("Attach File")
        self.attach_button.clicked.connect(self.attach_file)
        layout.addWidget(self.attach_button)

        self.execute_button = QPushButton("Execute")
        self.execute_button.clicked.connect(self.send_hash_packet)
        self.execute_button.setEnabled(False)
        layout.addWidget(self.execute_button)

        self.input_field.textChanged.connect(self.validate_input)

        self.setLayout(layout)

    def validate_input(self):
        data = self.input_field.toPlainText().encode()
        if len(data) > self.max_input_size:
            self.log_callback("Error: Input size exceeds 95% of 65536 bytes", "error")
            self.execute_button.setEnabled(False)
        else:
            self.execute_button.setEnabled(True)

    def attach_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Open Text File", "", "Text Files (*.txt)")
        if file_path:
            try:
                with open(file_path, 'r') as file:
                    content = file.read()
                    self.input_field.setPlainText(content)
            except Exception as e:
                self.log_callback(f"Failed to read file: {e}", "error")

    def send_hash_packet(self):
        algo_to_option = {
            "SHA224": 0x21, "SHA256": 0x22,
            "SHA384": 0x23, "SHA512": 0x24
        }
        option = algo_to_option[self.dropdown.currentText()]
        input_data = self.input_field.toPlainText().encode()
        input_size = len(input_data)

        txid = random.getrandbits(32).to_bytes(4, 'big')
        cmd = bytes([0x03])
        option_byte = bytes([option])
        input_size_bytes = input_size.to_bytes(2, 'big')
        eod = bytes.fromhex("DEADBEEF")

        packet = txid + cmd + option_byte + input_size_bytes + input_data + eod
        self.send_packet_callback(packet)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Hardware Security Module Interface")
        self.resize(1000, 700)

        self.serial = None
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.main_layout = QVBoxLayout()

        self.navbar = QListWidget()
        self.navbar.setFixedWidth(200)
        self.pages = QStackedWidget()

        self.navbar.addItem(QListWidgetItem("Device"))
        self.navbar.addItem(QListWidgetItem("RNG"))
        self.navbar.addItem(QListWidgetItem("Hashing"))

        self.device_page = DevicePage(self.handle_serial)
        self.rng_page = RNGPage(self.send_packet)
        self.hashing_page = HashingPage(self.send_packet, self.log)

        self.pages.addWidget(self.device_page)
        self.pages.addWidget(self.rng_page)
        self.pages.addWidget(self.hashing_page)

        self.navbar.currentRowChanged.connect(self.pages.setCurrentIndex)
        self.navbar.setCurrentRow(0)

        content_layout = QHBoxLayout()
        content_layout.addWidget(self.navbar)
        content_layout.addWidget(self.pages)

        self.terminal = QTextEdit()
        self.terminal.setReadOnly(True)
        self.terminal.setFixedHeight(200)
        self.terminal.setStyleSheet("background-color: black; color: white; font-family: monospace;")

        self.main_layout.addLayout(content_layout)
        self.main_layout.addWidget(self.terminal)
        self.central_widget.setLayout(self.main_layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.read_serial)

    def handle_serial(self, action):
        if action == "connect":
            try:
                # Change 'COM4' to your actual serial port
                self.serial = serial.Serial('COM4', 115200, timeout=0.1)
                self.log("[INFO] Connected to COM4", "info")
                self.timer.start(100)
            except Exception as e:
                self.log(f"[ERROR] Connection failed: {e}", "error")
        elif action == "disconnect":
            if self.serial and self.serial.is_open:
                self.serial.close()
                self.log("[INFO] Disconnected from COM4", "info")
                self.timer.stop()

    def send_packet(self, data):
        if self.serial and self.serial.is_open:
            self.serial.write(data)
            self.log(f"[TX] {' '.join(f'{b:02X}' for b in data)}", "tx")
        else:
            self.log("[WARNING] Serial not connected.", "warning")

    def read_serial(self):
        if self.serial and self.serial.in_waiting:
            data = self.serial.read(self.serial.in_waiting)
            self.log(f"[RX] {' '.join(f'{b:02X}' for b in data)}", "rx")

    def log(self, message, level="info"):
        color = {
            "info": "white",
            "warning": "orange",
            "error": "red",
            "tx": "cyan",
            "rx": "lime"
        }.get(level, "white")

        fmt = QTextCharFormat()
        fmt.setForeground(QColor(color))

        cursor = self.terminal.textCursor()
        cursor.movePosition(cursor.End)
        cursor.setCharFormat(fmt)
        cursor.insertText(message + "\n")
        self.terminal.setTextCursor(cursor)
        self.terminal.ensureCursorVisible()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
