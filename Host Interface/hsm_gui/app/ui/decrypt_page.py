# app/ui/decrypt_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton,
    QComboBox, QTextEdit, QFileDialog, QScrollArea
)
from PySide6.QtCore import Qt
from PySide6.QtGui import QIcon
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.core.iv_store import IVStore
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file, load_file_data
from app.core.logger import log
import os

iv_store = IVStore()

ICON_PATH = os.path.join(os.path.dirname(__file__), "..", "resources", "icons")

class DecryptPage(QWidget):
    def __init__(self, device: DeviceInterface):
        super().__init__()
        self.device = device

        layout = QVBoxLayout()

        def inline_row(label_text, widget):
            row = QHBoxLayout()
            row.setContentsMargins(12, 0, 12, 0)
            label = QLabel(label_text)
            label.setFixedWidth(120)
            row.addWidget(label)
            row.addWidget(widget)
            return row

        def make_icon_button(icon_file, tooltip):
            btn = QPushButton()
            btn.setIcon(QIcon(os.path.join(ICON_PATH, icon_file)))
            btn.setToolTip(tooltip)
            btn.setFixedSize(32, 32)
            btn.setStyleSheet("border: none;")
            return btn

        self.key_option = QComboBox()
        self.key_option.addItem("Select Key Size")
        self.key_option.addItems(["128-bit Key", "256-bit Key"])

        self.key_state = QComboBox()
        self.key_state.addItem("Select Key State")
        self.key_state.addItems(["Provide Key", "Provide Key ID"])

        self.iv_state = QComboBox()
        self.iv_state.addItem("Select IV State")
        self.iv_state.addItems(["Provide IV", "Lookup IV"])

        self.key_input = QTextEdit(); self.key_input.setFixedHeight(40)
        self.key_input.setPlaceholderText("Enter 16-byte Key or 4-byte Key ID")
        self.key_input.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.iv_input = QTextEdit(); self.iv_input.setFixedHeight(40)
        self.iv_input.setPlaceholderText("Enter 16-byte Initialization Vector")
        self.iv_input.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.ciphertext_input = QTextEdit()
        self.ciphertext_input.setPlaceholderText("Enter Ciphertext or Upload File")

        self.upload_btn = make_icon_button("upload", "Upload Ciphertext File")
        self.upload_btn.clicked.connect(self.upload_file)

        self.decrypt_btn = QPushButton("Decrypt")
        self.decrypt_btn.clicked.connect(self.perform_decryption)

        self.output_keyid = QTextEdit(); self.output_keyid.setReadOnly(True); self.output_keyid.setFixedHeight(30)
        self.output_keyid.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.output_iv = QTextEdit(); self.output_iv.setReadOnly(True); self.output_iv.setFixedHeight(30)
        self.output_iv.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.output_plaintext = QTextEdit(); self.output_plaintext.setReadOnly(True)

        self.copy_keyid_btn = make_icon_button("copy.png", "Copy Key ID")
        self.copy_keyid_btn.clicked.connect(lambda: copy_to_clipboard(self.output_keyid.toPlainText()))

        self.copy_iv_btn = make_icon_button("copy.png", "Copy IV")
        self.copy_iv_btn.clicked.connect(lambda: copy_to_clipboard(self.output_iv.toPlainText()))

        self.copy_plaintext_btn = make_icon_button("copy.png", "Copy Plaintext")
        self.copy_plaintext_btn.clicked.connect(lambda: copy_to_clipboard(self.output_plaintext.toPlainText()))

        self.save_plaintext_btn = make_icon_button("download.png", "Download Plaintext")
        self.save_plaintext_btn.clicked.connect(lambda: save_text_to_file(self.output_plaintext.toPlainText(), "plaintext.txt"))

        layout.addLayout(inline_row("Key Option:", self.key_option))
        layout.addLayout(inline_row("Key State:", self.key_state))
        layout.addLayout(inline_row("Key Data / ID:", self.key_input))
        layout.addLayout(inline_row("IV State:", self.iv_state))
        layout.addLayout(inline_row("IV Data:", self.iv_input))

        # Ciphertext upload row
        cipher_label_row = QHBoxLayout()
        cipher_label = QLabel("Ciphertext:")
        cipher_label.setFixedWidth(120)
        cipher_label_row.setContentsMargins(12, 0, 12, 0)
        cipher_label_row.addWidget(cipher_label)
        cipher_label_row.addWidget(self.upload_btn)
        cipher_label_row.addStretch()

        layout.addLayout(cipher_label_row)
        layout.setContentsMargins(12, 0, 12, 0)
        layout.addWidget(self.ciphertext_input)
        layout.addWidget(self.decrypt_btn)

        def output_row(label_text, output_widget, copy_btn):
            row = QHBoxLayout()
            row.setContentsMargins(12, 0, 12, 0)
            label = QLabel(label_text)
            label.setFixedWidth(120)
            row.addWidget(label)
            row.addWidget(output_widget)
            row.addStretch()
            row.addWidget(copy_btn)
            return row

        layout.addLayout(output_row("Key ID:", self.output_keyid, self.copy_keyid_btn))
        layout.addLayout(output_row("IV:", self.output_iv, self.copy_iv_btn))

        layout.addLayout(inline_row("Plaintext:", QLabel("")))
        plaintext_block = QHBoxLayout()
        plaintext_block.setContentsMargins(12, 0, 12, 10)
        plaintext_block.addWidget(self.output_plaintext)
        plaintext_block.addWidget(self.copy_plaintext_btn)
        plaintext_block.addWidget(self.save_plaintext_btn)
        layout.addLayout(plaintext_block)

        container = QWidget()
        container.setLayout(layout)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(container)

        outer_layout = QVBoxLayout(self)
        outer_layout.addWidget(scroll)

    def upload_file(self):
        content = load_file_data()
        if content:
            self.ciphertext_input.setPlainText(content)

    def perform_decryption(self):
        try:
            cmd = 0x02
            option = 0x11 if "128" in self.key_option.currentText() else 0x12
            key_state = 0xD1 if "BYOK" in self.key_state.currentText() else 0xD2
            iv_state = 0xC1 if "BYIV" in self.iv_state.currentText() else 0xC2

            key_data = bytearray(16)
            iv_data = bytearray(16)

            key_input_bytes = bytes.fromhex(self.key_input.toPlainText().strip())
            if key_state == 0xD1:
                key_data[:len(key_input_bytes)] = key_input_bytes
            elif key_state == 0xD2:
                key_data[:4] = key_input_bytes[:4]

            ciphertext = bytes.fromhex(self.ciphertext_input.toPlainText().strip())

            if iv_state == 0xC1:
                iv_input_bytes = bytes.fromhex(self.iv_input.toPlainText().strip())
                iv_data[:len(iv_input_bytes)] = iv_input_bytes
            elif iv_state == 0xC2:
                found_iv = iv_store.find_iv(key_data[:4], ciphertext)
                if found_iv is None:
                    self.output_plaintext.setPlainText("IV not found in database")
                    return
                iv_data = found_iv

            input_data = bytes([key_state]) + key_data + bytes([iv_state]) + iv_data + ciphertext
            packet, txid = build_packet(cmd, option, input_data)

            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            keyid = output_data[:4]
            iv = output_data[4:20]
            plaintext = output_data[20:]

            self.output_keyid.setPlainText(keyid.hex(" ").upper())
            self.output_iv.setPlainText(iv.hex(" ").upper())
            self.output_plaintext.setPlainText(plaintext.decode(errors="replace"))
            log(f"Decryption completed.")

        except Exception as e:
            log(f"Decryption Error: {str(e)}")
            self.output_plaintext.setPlainText("Error: " + str(e))