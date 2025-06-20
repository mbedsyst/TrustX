import sys
import serial
import random
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QPushButton, QTextEdit,
    QHBoxLayout, QStackedWidget, QListWidget, QListWidgetItem, QComboBox,
    QLabel, QFileDialog, QLineEdit
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

class EncryptionPage(QWidget):
    def __init__(self, send_packet_callback, log_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        self.log_callback = log_callback

        layout = QVBoxLayout()

        # Dropdowns for Algorithm and Key Size
        self.algo_dropdown = QComboBox()
        self.algo_dropdown.addItems(["AES"])
        layout.addWidget(QLabel("Select Algorithm:"))
        layout.addWidget(self.algo_dropdown)

        self.keysize_dropdown = QComboBox()
        self.keysize_dropdown.addItems(["128", "256"])
        layout.addWidget(QLabel("Select Key Size (bits):"))
        layout.addWidget(self.keysize_dropdown)

        # Key Option Dropdown
        self.key_option_dropdown = QComboBox()
        self.key_option_dropdown.addItems(["Generate by HSM", "Provide Key ID", "External Key"])
        layout.addWidget(QLabel("Select Key Option:"))
        layout.addWidget(self.key_option_dropdown)

        # Key Field
        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter 32-byte Key or Key ID here (hex encoded)")
        layout.addWidget(QLabel("Key / Key ID:"))
        layout.addWidget(self.key_input)

        # Input Data Field
        self.input_data_field = QTextEdit()
        self.input_data_field.setPlaceholderText("Plaintext input or attach a file")
        layout.addWidget(QLabel("Plaintext Input:"))
        layout.addWidget(self.input_data_field)

        # File Attachment
        self.attach_button = QPushButton("Attach File")
        self.attach_button.clicked.connect(self.attach_file)
        layout.addWidget(self.attach_button)

        # Encrypt Button
        self.encrypt_button = QPushButton("Encrypt Data")
        self.encrypt_button.clicked.connect(self.encrypt_data)
        layout.addWidget(self.encrypt_button)

        # Output Fields
        self.output_keyid = QLineEdit()
        self.output_keyid.setReadOnly(True)
        layout.addWidget(QLabel("Returned Key ID:"))
        layout.addWidget(self.output_keyid)

        self.output_iv = QLineEdit()
        self.output_iv.setReadOnly(True)
        layout.addWidget(QLabel("Returned IV:"))
        layout.addWidget(self.output_iv)

        self.output_cipher = QTextEdit()
        self.output_cipher.setReadOnly(True)
        layout.addWidget(QLabel("Ciphertext:"))
        layout.addWidget(self.output_cipher)

        btn_layout = QHBoxLayout()
        self.copy_btn = QPushButton("Copy")
        self.copy_btn.clicked.connect(self.copy_ciphertext)
        btn_layout.addWidget(self.copy_btn)

        self.download_btn = QPushButton("Download")
        self.download_btn.clicked.connect(self.download_ciphertext)
        btn_layout.addWidget(self.download_btn)

        layout.addLayout(btn_layout)
        self.setLayout(layout)

    def attach_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Open Text File", "", "Text Files (*.txt)")
        if file_path:
            try:
                with open(file_path, 'r') as file:
                    self.input_data_field.setPlainText(file.read())
            except Exception as e:
                self.log_callback(f"Failed to read file: {e}", "error")

    def copy_ciphertext(self):
        clipboard = QApplication.clipboard()
        clipboard.setText(self.output_cipher.toPlainText())

    def download_ciphertext(self):
        file_path, _ = QFileDialog.getSaveFileName(self, "Save Ciphertext", "ciphertext.txt", "Text Files (*.txt)")
        if file_path:
            try:
                with open(file_path, 'w') as f:
                    f.write(self.output_cipher.toPlainText())
            except Exception as e:
                self.log_callback(f"Failed to save file: {e}", "error")

    def encrypt_data(self):
        try:
            txid = random.getrandbits(32).to_bytes(4, 'big')
            cmd = bytes([0x01])
            option = bytes([0x01]) if self.keysize_dropdown.currentText() == "128" else bytes([0x02])

            key_option_index = self.key_option_dropdown.currentIndex()
            key_option_byte = bytes([key_option_index + 1])  # 0x01, 0x02, 0x03

            key_data = bytes.fromhex(self.key_input.toPlainText().strip())
            key_data = key_data.ljust(32, b'\x00') if key_option_index == 2 else b'\x00' * 32

            plaintext = self.input_data_field.toPlainText().encode()
            input_data = key_option_byte + key_data + plaintext
            input_size = len(input_data).to_bytes(2, 'big')

            eod = bytes.fromhex("DEADBEEF")
            packet = txid + cmd + option + input_size + input_data + eod

            self.send_packet_callback(packet)
            self.current_txid = txid  # Save to match in read_serial()
        except Exception as e:
            self.log_callback(f"Encryption failed: {e}", "error")

    def update_output(self, data: bytes):
        try:
            keyid = data[0:4].hex().upper()
            iv = data[4:20].hex().upper()
            ciphertext = data[20:].hex().upper()

            self.output_keyid.setText(keyid)
            self.output_iv.setText(iv)
            self.output_cipher.setPlainText(ciphertext)
        except Exception as e:
            self.log_callback(f"Failed to parse response: {e}", "error")

class DecryptionPage(QWidget):
    def __init__(self, send_packet_callback, log_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        self.log_callback = log_callback

        layout = QVBoxLayout()

        # Dropdowns for Algorithm and Key Size
        self.algo_dropdown = QComboBox()
        self.algo_dropdown.addItems(["AES"])
        layout.addWidget(QLabel("Select Algorithm:"))
        layout.addWidget(self.algo_dropdown)

        self.keysize_dropdown = QComboBox()
        self.keysize_dropdown.addItems(["128", "256"])
        layout.addWidget(QLabel("Select Key Size (bits):"))
        layout.addWidget(self.keysize_dropdown)

        # Key Option Dropdown
        self.key_option_dropdown = QComboBox()
        self.key_option_dropdown.addItems(["BYOK", "GYOK", "Stored Key"])
        layout.addWidget(QLabel("Key Option:"))
        layout.addWidget(self.key_option_dropdown)

        # Key Field
        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter 32-byte Key or Key ID (hex)")
        layout.addWidget(QLabel("Key / Key ID:"))
        layout.addWidget(self.key_input)

        # IV Option Dropdown
        self.iv_option_dropdown = QComboBox()
        self.iv_option_dropdown.addItems(["Provide Own IV", "From Database"])
        layout.addWidget(QLabel("IV Option:"))
        layout.addWidget(self.iv_option_dropdown)

        # IV Input
        self.iv_input = QTextEdit()
        self.iv_input.setPlaceholderText("Enter 16-byte IV (hex)")
        layout.addWidget(QLabel("IV (if applicable):"))
        layout.addWidget(self.iv_input)

        # Ciphertext Input
        self.ciphertext_field = QTextEdit()
        self.ciphertext_field.setPlaceholderText("Paste Ciphertext or attach file")
        layout.addWidget(QLabel("Ciphertext Input:"))
        layout.addWidget(self.ciphertext_field)

        # Attach File Button
        self.attach_button = QPushButton("Attach File")
        self.attach_button.clicked.connect(self.attach_file)
        layout.addWidget(self.attach_button)

        # Decrypt Button
        self.decrypt_button = QPushButton("Decrypt Data")
        self.decrypt_button.clicked.connect(self.decrypt_data)
        layout.addWidget(self.decrypt_button)

        # Output Fields
        self.output_keyid = QLineEdit()
        self.output_keyid.setReadOnly(True)
        layout.addWidget(QLabel("Returned Key ID:"))
        layout.addWidget(self.output_keyid)

        self.output_plain = QTextEdit()
        self.output_plain.setReadOnly(True)
        layout.addWidget(QLabel("Decrypted Plaintext:"))
        layout.addWidget(self.output_plain)

        btn_layout = QHBoxLayout()
        self.copy_btn = QPushButton("Copy")
        self.copy_btn.clicked.connect(self.copy_plaintext)
        btn_layout.addWidget(self.copy_btn)

        self.download_btn = QPushButton("Download")
        self.download_btn.clicked.connect(self.download_plaintext)
        btn_layout.addWidget(self.download_btn)

        layout.addLayout(btn_layout)
        self.setLayout(layout)

    def attach_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Open Ciphertext File", "", "Text Files (*.txt)")
        if file_path:
            try:
                with open(file_path, 'r') as file:
                    self.ciphertext_field.setPlainText(file.read())
            except Exception as e:
                self.log_callback(f"Failed to read file: {e}", "error")

    def copy_plaintext(self):
        clipboard = QApplication.clipboard()
        clipboard.setText(self.output_plain.toPlainText())

    def download_plaintext(self):
        file_path, _ = QFileDialog.getSaveFileName(self, "Save Decrypted Plaintext", "plaintext.txt", "Text Files (*.txt)")
        if file_path:
            try:
                with open(file_path, 'w') as f:
                    f.write(self.output_plain.toPlainText())
            except Exception as e:
                self.log_callback(f"Failed to save file: {e}", "error")

    def decrypt_data(self):
        try:
            txid = random.getrandbits(32).to_bytes(4, 'big')
            cmd = bytes([0x02])
            option = bytes([0x01])  # Assuming CBC mode for now

            key_option_index = self.key_option_dropdown.currentIndex()
            key_option_byte = bytes([key_option_index + 1])

            key_data = bytes.fromhex(self.key_input.toPlainText().strip())
            key_data = key_data.ljust(32, b'\x00') if key_option_index == 0 else key_data[:32]

            iv_option_index = self.iv_option_dropdown.currentIndex()
            iv_option_byte = bytes([iv_option_index + 1])

            iv_data = bytes.fromhex(self.iv_input.toPlainText().strip()) if iv_option_index == 0 else b'\x00' * 16
            iv_data = iv_data[:16].ljust(16, b'\x00')

            ciphertext = bytes.fromhex(self.ciphertext_field.toPlainText().strip())

            input_data = key_option_byte + key_data + iv_option_byte + iv_data + ciphertext
            input_size = len(input_data).to_bytes(2, 'big')

            eod = bytes.fromhex("DEADBEEF")
            packet = txid + cmd + option + input_size + input_data + eod

            self.send_packet_callback(packet)
            self.current_txid = txid
        except Exception as e:
            self.log_callback(f"Decryption failed: {e}", "error")

    def update_output(self, data: bytes):
        try:
            keyid = data[0:4].hex().upper()
            plaintext = data[4:].decode(errors='ignore')

            self.output_keyid.setText(keyid)
            self.output_plain.setPlainText(plaintext)
        except Exception as e:
            self.log_callback(f"Failed to parse decrypted output: {e}", "error")

class RNGPage(QWidget):
    def __init__(self, send_packet_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        layout = QVBoxLayout()

        self.dropdown = QComboBox()
        self.dropdown.addItems(["4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096"])
        layout.addWidget(QLabel("Select Byte Size for RNG:"))
        layout.addWidget(self.dropdown)

        self.execute_button = QPushButton("Generate Random Number")
        self.execute_button.clicked.connect(self.send_rng_packet)
        layout.addWidget(self.execute_button)

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setPlaceholderText("Random number will appear here...")
        layout.addWidget(QLabel("Generated Random Number:"))
        layout.addWidget(self.output_field)

        self.setLayout(layout)

    def send_rng_packet(self):
        size_to_option = {
            "4": 0x31, "8": 0x32, "16": 0x33, "32": 0x35, "64": 0x37,
            "128": 0x39, "256": 0x3A, "512": 0x3C, "1024": 0x3D,
            "2048": 0x3E, "4096": 0x3F
        }
        selected_size = self.dropdown.currentText()
        option = size_to_option[selected_size]
        txid = random.getrandbits(32).to_bytes(4, 'big')
        cmd = bytes([0x04])  # Command code for RNG
        option_byte = bytes([option])
        input_size = b"\x00\x00"
        eod = bytes.fromhex("DEADBEEF")

        self.current_txid = txid  # Store for matching response
        packet = txid + cmd + option_byte + input_size + eod
        self.send_packet_callback(packet)

    def update_rng_output(self, data: bytes):
        self.output_field.setPlainText(data.hex().upper())

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

class OTPPage(QWidget):
    def __init__(self, send_packet_callback, log_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        self.log_callback = log_callback

        layout = QVBoxLayout()

        self.dropdown = QComboBox()
        self.dropdown.addItems(["16", "32", "64", "128", "256", "512", "1024"])
        layout.addWidget(QLabel("Select Byte Size for OTP:"))
        layout.addWidget(self.dropdown)

        self.execute_button = QPushButton("Generate OTP")
        self.execute_button.clicked.connect(self.send_otp_packet)
        layout.addWidget(self.execute_button)

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setPlaceholderText("OTP will appear here after generation...")
        layout.addWidget(QLabel("Generated OTP:"))
        layout.addWidget(self.output_field)

        self.setLayout(layout)

    def send_otp_packet(self):
        size_to_option = {
            "16": 0x33, "32": 0x35, "64": 0x37, "128": 0x39,
            "256": 0x3A, "512": 0x3C, "1024": 0x3D
        }
        selected_size = self.dropdown.currentText()
        option = size_to_option[selected_size]
        txid = random.getrandbits(32).to_bytes(4, 'big')
        cmd = bytes([0x05])  # Assuming different command code for OTP
        option_byte = bytes([option])
        input_size = b"\x00\x00"
        eod = bytes.fromhex("DEADBEEF")
        packet = txid + cmd + option_byte + input_size + eod

        self.current_txid = txid  # to match the response later
        self.send_packet_callback(packet)

    def update_otp_output(self, data):
        # Normalize hex string to bytes
        if isinstance(data, str):
            try:
                data = bytes.fromhex(data)
            except ValueError:
                self.output_field.setPlainText("Invalid data format!")
                return

        # Check minimum length: TXID (4) + SIZE (2) + at least 1 byte + EOD (4)
        if len(data) < 10:
            self.output_field.setPlainText("Invalid response! Length too short.")
            return

        txid = data[0:4]
        output_size = int.from_bytes(data[4:6], 'big')

        expected_len = 6 + output_size + 4  # TXID + size + data + EOD
        if len(data) < expected_len:
            self.output_field.setPlainText(
                f"Invalid response! Expected {expected_len} bytes, got {len(data)}.\n"
                f"Raw: {data.hex().upper()}"
            )
            return

        otp_output = data[6:6 + output_size]
        eod_flag = data[6 + output_size: 6 + output_size + 4]

        if eod_flag != bytes.fromhex("CAFEBABE"):
            self.output_field.setPlainText(
                f"Invalid EOD Flag! Got {eod_flag.hex().upper()}"
            )
            return

        self.output_field.setPlainText(otp_output.hex().upper())

class KeyGenPage(QWidget):
    def __init__(self, send_packet_callback, log_callback):
        super().__init__()
        self.send_packet_callback = send_packet_callback
        self.log_callback = log_callback

        layout = QVBoxLayout()

        self.dropdown = QComboBox()
        self.dropdown.addItems(["16", "24", "32", "48", "64", "66", "128", "256", "512"])
        layout.addWidget(QLabel("Select Key Size (bytes):"))
        layout.addWidget(self.dropdown)

        self.execute_button = QPushButton("Generate Key")
        self.execute_button.clicked.connect(self.send_keygen_packet)
        layout.addWidget(self.execute_button)

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setPlaceholderText("Key ID will appear here...")
        layout.addWidget(QLabel("Returned Key ID:"))
        layout.addWidget(self.output_field)

        self.setLayout(layout)

    def send_keygen_packet(self):
        size_to_option = {
            "16": 0x33, "24": 0x34, "32": 0x35, "48": 0x36,
            "64": 0x37, "66": 0x38, "128": 0x39, "256": 0x3A, "512": 0x3C
        }
        selected_size = self.dropdown.currentText()
        option = size_to_option[selected_size]
        txid = random.getrandbits(32).to_bytes(4, 'big')
        cmd = bytes([0x06])  # Assuming different command code for Key Gen
        option_byte = bytes([option])
        input_size = b"\x00\x00"
        eod = bytes.fromhex("DEADBEEF")
        packet = txid + cmd + option_byte + input_size + eod

        self.current_txid = txid  # track txid for response matching
        self.send_packet_callback(packet)

    def update_keyid_output(self, keyid: str):
        self.output_field.setPlainText(keyid)

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
        self.navbar.addItem(QListWidgetItem("Encryption"))
        self.navbar.addItem(QListWidgetItem("Decryption"))
        self.navbar.addItem(QListWidgetItem("RNG"))
        self.navbar.addItem(QListWidgetItem("Hashing"))
        self.navbar.addItem(QListWidgetItem("One-Time Pad"))
        self.navbar.addItem(QListWidgetItem("Key Generation"))

        self.device_page = DevicePage(self.handle_serial)
        self.encryption_page = EncryptionPage(self.send_packet, self.log)
        self.decryption_page = DecryptionPage(self.send_packet, self.log)
        self.rng_page = RNGPage(self.send_packet)
        self.hashing_page = HashingPage(self.send_packet, self.log)
        self.otp_page = OTPPage(self.send_packet, self.log)
        self.keygen_page = KeyGenPage(self.send_packet, self.log)

        self.pages.addWidget(self.device_page)
        self.pages.addWidget(self.encryption_page)
        self.pages.addWidget(self.decryption_page)
        self.pages.addWidget(self.rng_page)
        self.pages.addWidget(self.hashing_page)
        self.pages.addWidget(self.otp_page)
        self.pages.addWidget(self.keygen_page)

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
        try:
            if self.serial and self.serial.in_waiting:
                data = self.serial.read(self.serial.in_waiting)
                self.log(f"[RX] {' '.join(f'{b:02X}' for b in data)}", "rx")

                # Example response processing
                current_page = self.pages.currentWidget()

                if isinstance(current_page, RNGPage):
                    if len(data) > 8:
                        rng_data = data[8:-4]  # Remove txid+cmd+opt and EOD
                        current_page.update_rng_output(rng_data)

                elif isinstance(current_page, EncryptionPage):
                    if len(data) > 8:
                        try:
                            output_size = int.from_bytes(data[4:6], 'big')
                            output_data = data[6:6 + output_size]
                            current_page.update_output(output_data)
                        except Exception as e:
                            self.log(f"[ERROR] Failed to parse encryption output: {e}", "error")
                
                elif isinstance(current_page, DecryptionPage):
                    if len(data) > 8:
                        plaintext_data = data[8:-4]  # Remove TXID+SIZE, keep OUTPUT DATA
                        current_page.update_output(plaintext_data)

                elif isinstance(current_page, OTPPage):
                    if len(data) > 8:
                        otp_bytes = data[8:-4]
                        current_page.update_otp_output(otp_bytes)

                elif isinstance(current_page, KeyGenPage):
                    if len(data) > 8:
                        keyid = data[8:-4].hex().upper()
                        current_page.update_keyid_output(keyid)

        except Exception as e:
            self.log(f"[ERROR] Serial read failed: {e}", "error")


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
