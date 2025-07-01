# app/ui/keystore_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QPushButton, QTextEdit,
    QVBoxLayout, QHBoxLayout, QComboBox, QMessageBox, QSizePolicy
)
from PySide6.QtGui import QIcon, QFont
from PySide6.QtCore import Qt
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log
import os

ICON_PATH = "app/resources/icons"

class KeyStorePage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = DeviceInterface()

        layout = QVBoxLayout()

        self.usage_box = QComboBox()
        self.usage_box.addItem("Select Key Usage Type")
        self.usage_box.addItems(["Encrypt", "Decrypt", "HMAC"])
        self.usage_map = {
            "Encrypt": 0x01,
            "Decrypt": 0x02,
            "HMAC": 0x04,
        }
        layout.addWidget(self.usage_box)

        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter Key Value")
        self.key_input.setFixedHeight(40)
        self.key_input.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.key_input.setFont(QFont("Courier New", 9))
        layout.addWidget(self.key_input)

        self.store_btn = QPushButton("Store Key")
        self.store_btn.clicked.connect(self.store_key)
        layout.addWidget(self.store_btn)

        layout.addWidget(QLabel("Returned Key ID:"))

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setFixedHeight(60)
        self.output_field.setFont(QFont("Courier New", 9))
        self.output_field.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        output_layout = QHBoxLayout()
        output_layout.addWidget(self.output_field)

        button_layout = QVBoxLayout()
        self.copy_btn = QPushButton()
        self.copy_btn.setIcon(QIcon(os.path.join(ICON_PATH, "copy.png")))
        self.copy_btn.clicked.connect(self.copy_output)

        self.save_btn = QPushButton()
        self.save_btn.setIcon(QIcon(os.path.join(ICON_PATH, "download.png")))
        self.save_btn.clicked.connect(self.save_output)

        button_layout.addWidget(self.copy_btn)
        button_layout.addWidget(self.save_btn)
        output_layout.addLayout(button_layout)

        layout.addLayout(output_layout)
        layout.addStretch()
        self.setLayout(layout)

    def store_key(self):
        try:
            key_hex = self.key_input.toPlainText().strip().replace(" ", "")
            if len(key_hex) != 32:
                raise ValueError("Key must be exactly 16 bytes in hex (32 characters).")

            key_data = bytes.fromhex(key_hex)
            key_usage = self.usage_map.get(self.usage_box.currentText(), None)

            if key_usage is None:
                raise ValueError("Please select a valid Key Usage Type.")

            cmd = 0x08
            option = 0x00
            input_data = bytes([key_usage]) + key_data

            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            key_id = output_data[:4]
            hex_key_id = " ".join(f"{b:02X}" for b in key_id)
            self.output_field.setPlainText(hex_key_id)
            log(f"Stored Key ID: {hex_key_id}")

        except Exception as e:
            log(f"Key Store Error: {str(e)}")
            QMessageBox.critical(self, "Error", str(e))

    def copy_output(self):
        copy_to_clipboard(self.output_field.toPlainText())

    def save_output(self):
        save_text_to_file(self.output_field.toPlainText(), default_name="key_id.txt")


"""from PySide6.QtWidgets import (
    QWidget, QLabel, QPushButton, QTextEdit,
    QVBoxLayout, QHBoxLayout, QComboBox, QMessageBox, QScrollArea
)
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log

class KeyStorePage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = DeviceInterface()

        self.usage_box = QComboBox()
        self.usage_box.addItems(["Encrypt", "Decrypt", "HMAC"])
        self.usage_map = {
            "Encrypt": 0x01,
            "Decrypt": 0x02,
            "HMAC": 0x04,
        }

        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter 16-byte Key in Hex (e.g. 00112233445566778899AABBCCDDEEFF)")

        self.store_btn = QPushButton("Store Key")
        self.store_btn.clicked.connect(self.store_key)

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)

        self.copy_btn = QPushButton("Copy")
        self.copy_btn.clicked.connect(self.copy_output)

        self.save_btn = QPushButton("Download")
        self.save_btn.clicked.connect(self.save_output)

        btn_layout = QHBoxLayout()
        btn_layout.addWidget(self.copy_btn)
        btn_layout.addWidget(self.save_btn)

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Select Key Usage Type:"))
        layout.addWidget(self.usage_box)
        layout.addWidget(QLabel("Enter Key Data (16 bytes):"))
        layout.addWidget(self.key_input)
        layout.addWidget(self.store_btn)
        layout.addWidget(QLabel("Returned Key ID:"))
        layout.addWidget(self.output_field)
        layout.addLayout(btn_layout)

        container = QWidget()
        container.setLayout(layout)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(container)

        outer_layout = QVBoxLayout(self)
        outer_layout.addWidget(scroll)

    def store_key(self):
        try:
            key_hex = self.key_input.toPlainText().strip().replace(" ", "")
            if len(key_hex) != 32:
                raise ValueError("Key must be exactly 16 bytes in hex (32 characters).")

            key_data = bytes.fromhex(key_hex)
            key_usage = self.usage_map[self.usage_box.currentText()]

            cmd = 0x08
            option = 0x00
            input_data = bytes([key_usage]) + key_data

            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            key_id = output_data[:4]
            hex_key_id = " ".join(f"{b:02X}" for b in key_id)
            self.output_field.setPlainText(hex_key_id)
            log(f"Stored Key ID: {hex_key_id}")

        except Exception as e:
            log(f"Key Store Error: {str(e)}")
            QMessageBox.critical(self, "Error", str(e))

    def copy_output(self):
        copy_to_clipboard(self.output_field.toPlainText())

    def save_output(self):
        save_text_to_file(self.output_field.toPlainText(), default_name="key_id.txt")"""
