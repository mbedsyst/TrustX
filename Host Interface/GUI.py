import sys
import random
import struct
import serial
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QTextEdit, QStackedWidget, QListWidget, QListWidgetItem,
    QComboBox
)
from PyQt5.QtCore import Qt, QTimer, QTime
from PyQt5.QtGui import QFont, QColor, QPalette

class DashboardPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()

        self.banner = QLabel("Welcome to the HSM Interface Dashboard")
        self.banner.setFont(QFont("Arial", 18, QFont.Bold))
        self.banner.setAlignment(Qt.AlignCenter)

        self.clock = QLabel()
        self.clock.setFont(QFont("Courier", 14))
        self.clock.setAlignment(Qt.AlignCenter)

        layout.addWidget(self.banner)
        layout.addWidget(self.clock)
        self.setLayout(layout)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_time)
        self.timer.start(1000)
        self.update_time()

    def update_time(self):
        current_time = QTime.currentTime().toString("hh:mm:ss AP")
        self.clock.setText(f"Current Time: {current_time}")

class DevicePage(QWidget):
    def __init__(self, log_callback):
        super().__init__()
        self.log_callback = log_callback
        layout = QVBoxLayout()

        button_layout = QHBoxLayout()

        self.connect_button = QPushButton("Connect")
        self.connect_button.setStyleSheet("background-color: #0A1F44; color: white; padding: 5px;")
        self.connect_button.setFixedWidth(100)
        self.connect_button.clicked.connect(self.connect_serial)

        self.disconnect_button = QPushButton("Disconnect")
        self.disconnect_button.setStyleSheet("background-color: #0A1F44; color: white; padding: 5px;")
        self.disconnect_button.setFixedWidth(100)
        self.disconnect_button.clicked.connect(self.disconnect_serial)

        button_layout.addWidget(self.connect_button)
        button_layout.addWidget(self.disconnect_button)
        layout.addLayout(button_layout)

        self.setLayout(layout)
        self.serial = None
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.read_serial)

    def connect_serial(self):
        try:
            self.serial = serial.Serial("COM4", 115200, timeout=0.5)
            self.timer.start(100)
            self.log_callback("[INFO] Connected to COM4 at 115200", color="#00BFFF")
        except Exception as e:
            self.log_callback(f"[ERROR] {str(e)}", color="#FF5555")

    def disconnect_serial(self):
        if self.serial and self.serial.is_open:
            self.serial.close()
            self.log_callback("[INFO] Disconnected from COM4", color="#00BFFF")
            self.timer.stop()

    def read_serial(self):
        if self.serial and self.serial.in_waiting:
            data = self.serial.read(self.serial.in_waiting)
            hex_data = ' '.join(f'{b:02X}' for b in data)
            self.log_callback(f"[RX] {hex_data}", color="#00FF00")

class RNGPage(QWidget):
    def __init__(self, log_callback, get_serial_callback):
        super().__init__()
        self.log_callback = log_callback
        self.get_serial = get_serial_callback

        layout = QVBoxLayout()

        self.byte_size_selector = QComboBox()
        self.byte_size_selector.addItems(["4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096"])
        layout.addWidget(QLabel("Select Byte Size:"))
        layout.addWidget(self.byte_size_selector)

        self.send_button = QPushButton("Execute")
        self.send_button.clicked.connect(self.send_rng_command)
        layout.addWidget(self.send_button)

        self.setLayout(layout)

    def send_rng_command(self):
        byte_size = int(self.byte_size_selector.currentText())
        txid = random.getrandbits(32)
        cmd = 0x04
        option_lookup = {
            4: 0x31, 8: 0x32, 16: 0x33, 32: 0x34,
            64: 0x35, 128: 0x36, 256: 0x37, 512: 0x38,
            1024: 0x39, 2048: 0x3A, 4096: 0x3B
        }
        option = option_lookup[byte_size]

        # Format: TXID (4) | CMD (1) | OPTION (1) | 00 00 00 | DE AD BE EF (not swapped)
        packet = struct.pack("<IBB", txid, cmd, option) + b"\x00\x00\x00" + b"\xDE\xAD\xBE\xEF"

        serial_port = self.get_serial()
        if serial_port and serial_port.is_open:
            serial_port.write(packet)
            hex_data = ' '.join(f'{b:02X}' for b in packet)
            self.log_callback(f"[TX] {hex_data}", color="#FFFF00")
        else:
            self.log_callback("[ERROR] Serial not connected.", color="#FF5555")

class HSMInterface(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HSM Debug Terminal")
        self.resize(1000, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        main_layout = QVBoxLayout(self.central_widget)

        top_layout = QHBoxLayout()

        self.nav_list = QListWidget()
        self.nav_list.setFixedWidth(200)
        for page_name in ["Dashboard", "Device", "Encryption", "Decryption", "Hashing", "HMAC", "Random Number", "Key Management", "About"]:
            QListWidgetItem(page_name, self.nav_list)

        self.pages = QStackedWidget()
        self.dashboard_page = DashboardPage()
        self.device_page = DevicePage(self.log_to_terminal)
        self.rng_page = RNGPage(self.log_to_terminal, self.get_serial)

        self.pages.addWidget(self.dashboard_page)  # Index 0: Dashboard
        self.pages.addWidget(self.device_page)     # Index 1: Device
        for i in range(4):                         # Index 2 to 5: Empty Pages
            empty_page = QLabel("Coming Soon...")
            empty_page.setAlignment(Qt.AlignCenter)
            self.pages.addWidget(empty_page)
        self.pages.addWidget(self.rng_page)        # Index 6: RNG Page
        for i in range(2):                         # Index 7 to 8: Empty Pages
            empty_page = QLabel("Coming Soon...")
            empty_page.setAlignment(Qt.AlignCenter)
            self.pages.addWidget(empty_page)

        self.nav_list.currentRowChanged.connect(self.pages.setCurrentIndex)

        top_layout.addWidget(self.nav_list)
        top_layout.addWidget(self.pages)

        self.terminal = QTextEdit()
        self.terminal.setReadOnly(True)
        self.terminal.setStyleSheet("background-color: #161B22; color: white;")
        self.terminal.setFixedHeight(150)

        main_layout.addLayout(top_layout)
        main_layout.addWidget(self.terminal)

        self.apply_dark_theme()

    def get_serial(self):
        return self.device_page.serial

    def log_to_terminal(self, message, color="#FFFFFF"):
        self.terminal.setTextColor(QColor(color))
        self.terminal.append(message)

    def apply_dark_theme(self):
        dark_palette = QPalette()
        dark_palette.setColor(QPalette.Window, QColor("#0D1117"))
        dark_palette.setColor(QPalette.WindowText, Qt.white)
        dark_palette.setColor(QPalette.Base, QColor("#161B22"))
        dark_palette.setColor(QPalette.AlternateBase, QColor("#0D1117"))
        dark_palette.setColor(QPalette.ToolTipBase, Qt.white)
        dark_palette.setColor(QPalette.ToolTipText, Qt.white)
        dark_palette.setColor(QPalette.Text, Qt.white)
        dark_palette.setColor(QPalette.Button, QColor("#21262D"))
        dark_palette.setColor(QPalette.ButtonText, Qt.white)
        dark_palette.setColor(QPalette.BrightText, Qt.red)
        dark_palette.setColor(QPalette.Highlight, QColor("#238636"))
        dark_palette.setColor(QPalette.HighlightedText, Qt.black)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = HSMInterface()
    window.show()
    sys.exit(app.exec_())
