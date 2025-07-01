# app/ui/keydelete_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QLineEdit, QPushButton,
    QVBoxLayout, QHBoxLayout, QMessageBox
)
from PySide6.QtGui import QIcon, QFont
from PySide6.QtCore import Qt
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.core.logger import log
from app.utils.file_utils import save_text_to_file
from app.utils.clipboard_utils import copy_to_clipboard
import os

ICON_PATH = "app/resources/icons"

class KeyDeletePage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = DeviceInterface()

        layout = QVBoxLayout()

        self.key_id_input = QLineEdit()
        self.key_id_input.setPlaceholderText("Enter 4-byte Key ID (8 hex characters)")
        layout.addWidget(QLabel("Key ID (Hex):"))
        layout.addWidget(self.key_id_input)

        self.delete_btn = QPushButton("Delete Key")
        self.delete_btn.clicked.connect(self.delete_key)
        layout.addWidget(self.delete_btn)

        layout.addWidget(QLabel("Status:"))

        self.result_label = QLabel("")
        self.result_label.setFont(QFont("Courier New", 9))
        self.result_label.setStyleSheet("font-weight: bold; border: 1px solid white; padding: 6px;")
        self.result_label.setWordWrap(True)
        layout.addWidget(self.result_label)

        button_layout = QHBoxLayout()

        self.copy_btn = QPushButton()
        self.copy_btn.setIcon(QIcon(os.path.join(ICON_PATH, "copy.png")))
        self.copy_btn.clicked.connect(self.copy_result)
        self.copy_btn.setEnabled(False)

        self.save_btn = QPushButton()
        self.save_btn.setIcon(QIcon(os.path.join(ICON_PATH, "download.png")))
        self.save_btn.clicked.connect(self.save_result)
        self.save_btn.setEnabled(False)

        button_layout.addStretch()
        button_layout.addWidget(self.copy_btn)
        button_layout.addWidget(self.save_btn)
        layout.addLayout(button_layout)

        layout.addStretch()
        self.setLayout(layout)

    def delete_key(self):
        key_id_hex = self.key_id_input.text().strip()
        if len(key_id_hex) != 8:
            QMessageBox.warning(self, "Input Error", "Key ID must be 4 bytes (8 hex characters).")
            return

        try:
            key_id = bytes.fromhex(key_id_hex)
        except ValueError:
            QMessageBox.warning(self, "Input Error", "Invalid hex input for Key ID.")
            return

        cmd = 0x09
        option = 0x00
        input_data = key_id

        try:
            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            if output_data == b"\x00\x00":
                msg = "Key Deleted Successfully."
            else:
                msg = f"Unexpected response: {output_data.hex().upper()}"

            self.result_label.setText(msg)
            log(f"Key Delete Result: {msg}")
            self.copy_btn.setEnabled(True)
            self.save_btn.setEnabled(True)

        except Exception as e:
            error_msg = f"Error: {str(e)}"
            self.result_label.setText(error_msg)
            log(error_msg)

    def copy_result(self):
        copy_to_clipboard(self.result_label.text())

    def save_result(self):
        save_text_to_file(self.result_label.text(), default_name="key_delete_result.txt")


"""from PySide6.QtWidgets import (
    QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QHBoxLayout, QMessageBox, QScrollArea
)
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.core.logger import log
from app.utils.file_utils import save_text_to_file
from app.utils.clipboard_utils import copy_to_clipboard

class KeyDeletePage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = DeviceInterface()

        self.key_id_input = QLineEdit()
        self.key_id_input.setPlaceholderText("Enter 4-byte Key ID (8 hex characters)")

        self.delete_btn = QPushButton("Delete Key")
        self.delete_btn.clicked.connect(self.delete_key)

        self.result_label = QLabel("")
        self.result_label.setStyleSheet("font-weight: bold;")

        self.copy_btn = QPushButton("Copy Result")
        self.copy_btn.clicked.connect(self.copy_result)
        self.copy_btn.setEnabled(False)

        self.save_btn = QPushButton("Download Result")
        self.save_btn.clicked.connect(self.save_result)
        self.save_btn.setEnabled(False)

        btn_layout = QHBoxLayout()
        btn_layout.addWidget(self.copy_btn)
        btn_layout.addWidget(self.save_btn)

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Key ID (Hex):"))
        layout.addWidget(self.key_id_input)
        layout.addWidget(self.delete_btn)
        layout.addWidget(QLabel("Status:"))
        layout.addWidget(self.result_label)
        layout.addLayout(btn_layout)

        container = QWidget()
        container.setLayout(layout)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(container)

        outer_layout = QVBoxLayout(self)
        outer_layout.addWidget(scroll)

    def delete_key(self):
        key_id_hex = self.key_id_input.text().strip()
        if len(key_id_hex) != 8:
            QMessageBox.warning(self, "Input Error", "Key ID must be 4 bytes (8 hex characters).")
            return

        try:
            key_id = bytes.fromhex(key_id_hex)
        except ValueError:
            QMessageBox.warning(self, "Input Error", "Invalid hex input for Key ID.")
            return

        cmd = 0x09
        option = 0x00
        input_data = key_id

        try:
            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            if output_data == b"\x00\x00":
                msg = "Key Deleted Successfully."
            else:
                msg = f"Unexpected response: {output_data.hex().upper()}"

            self.result_label.setText(msg)
            log(f"Key Delete Result: {msg}")
            self.copy_btn.setEnabled(True)
            self.save_btn.setEnabled(True)

        except Exception as e:
            error_msg = f"Error: {str(e)}"
            self.result_label.setText(error_msg)
            log(error_msg)

    def copy_result(self):
        copy_to_clipboard(self.result_label.text())

    def save_result(self):
        save_text_to_file(self.result_label.text(), default_name="key_delete_result.txt")"""
