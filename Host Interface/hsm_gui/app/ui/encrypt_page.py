# app/ui/encrypt_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton,
    QComboBox, QTextEdit, QFileDialog, QScrollArea, QSpacerItem, QSizePolicy
)
from PySide6.QtCore import Qt
from PySide6.QtGui import QIcon
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.iv_store import IVStore
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log
import os

iv_store = IVStore()

CMD_ENCRYPT = 0x01

KEY_OPTIONS = {
    "128-bit Key": 0x01,
    "256-bit Key": 0x02,
}

KEY_STATES = {
    "BYOK (User Provides Key)": 0xF1,
    "GYOK (HSM Generates Key)": 0xF2,
    "DABA (User Provides Key ID)": 0xF3,
}

IV_STATES = {
    "BYIV (User Provides IV)": 0xE1,
    "GYIV (HSM Generates IV)": 0xE2,
}

ICON_PATH = os.path.join(os.path.dirname(__file__), "..", "resources", "icons")

class EncryptPage(QWidget):
    def __init__(self, device):
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

        self.key_option = QComboBox()
        self.key_option.addItem("Select Key Size")
        self.key_option.addItems(KEY_OPTIONS.keys())

        self.key_state = QComboBox()
        self.key_state.addItem("Select Key State")
        self.key_state.addItems(KEY_STATES.keys())

        self.iv_state = QComboBox()
        self.iv_state.addItem("Select IV State")
        self.iv_state.addItems(IV_STATES.keys())

        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter 16-byte Key or 4-byte Key ID")
        self.key_input.setFixedHeight(40)
        self.key_input.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.iv_input = QTextEdit()
        self.iv_input.setPlaceholderText("Enter 16-byte Initialization Vector")
        self.iv_input.setFixedHeight(40)
        self.iv_input.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.plaintext_input = QTextEdit()
        self.plaintext_input.setPlaceholderText("Enter Plaintext or Upload file")

        self.upload_btn = QPushButton()
        self.upload_btn.setIcon(QIcon(os.path.join(ICON_PATH, "upload")))
        self.upload_btn.setFixedSize(32, 32)
        self.upload_btn.clicked.connect(self.upload_file)

        self.encrypt_btn = QPushButton("Encrypt")
        self.encrypt_btn.clicked.connect(self.encrypt_data)

        layout.addLayout(inline_row("Key Option:", self.key_option))
        layout.addLayout(inline_row("Key State:", self.key_state))
        layout.addLayout(inline_row("Key Data / ID:", self.key_input))
        layout.addLayout(inline_row("IV State:", self.iv_state))
        layout.addLayout(inline_row("IV Data:", self.iv_input))

        # Plaintext row
        plaintext_row = QHBoxLayout()
        plaintext_row.setContentsMargins(12, 0, 12, 0)
        label_plain = QLabel("Plaintext:")
        label_plain.setFixedWidth(120)
        plaintext_row.addWidget(label_plain)
        plaintext_row.addWidget(self.upload_btn)
        plaintext_row.addStretch()

        layout.addLayout(plaintext_row)
        layout.setContentsMargins(12, 0, 12, 0)
        layout.addWidget(self.plaintext_input)
        layout.addWidget(self.encrypt_btn)

        def output_row(label_text, output_widget, copy_btn, save_btn):
            row = QHBoxLayout()
            row.setContentsMargins(12, 0, 12, 0)
            label = QLabel(label_text)
            label.setFixedWidth(120)
            output_widget.setFixedHeight(30)
            output_widget.setMaximumHeight(30)
            output_widget.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
            row.addWidget(label)
            row.addWidget(output_widget)
            row.addStretch()
            row.addWidget(copy_btn)
            row.addWidget(save_btn)
            return row

        def make_icon_button(icon_file, tooltip):
            btn = QPushButton()
            btn.setIcon(QIcon(os.path.join(ICON_PATH, icon_file)))
            btn.setToolTip(tooltip)
            btn.setFixedSize(32, 32)
            btn.setStyleSheet("border: none;")
            return btn

        self.output_keyid = QTextEdit(); self.output_keyid.setReadOnly(True)
        self.output_iv = QTextEdit(); self.output_iv.setReadOnly(True)
        self.output_ciphertext = QTextEdit(); self.output_ciphertext.setReadOnly(True)

        self.copy_keyid = make_icon_button("copy.png", "Copy Key ID")
        self.copy_keyid.clicked.connect(lambda: copy_to_clipboard(self.output_keyid.toPlainText()))
        self.save_keyid = make_icon_button("download.png", "Download Key ID")
        self.save_keyid.clicked.connect(lambda: save_text_to_file(self.output_keyid.toPlainText(), "key_id.txt"))

        self.copy_iv = make_icon_button("copy.png", "Copy IV")
        self.copy_iv.clicked.connect(lambda: copy_to_clipboard(self.output_iv.toPlainText()))
        self.save_iv = make_icon_button("download.png", "Download IV")
        self.save_iv.clicked.connect(lambda: save_text_to_file(self.output_iv.toPlainText(), "iv.txt"))

        self.copy_cipher = make_icon_button("copy.png", "Copy Ciphertext")
        self.copy_cipher.clicked.connect(lambda: copy_to_clipboard(self.output_ciphertext.toPlainText()))
        self.save_cipher = make_icon_button("download.png", "Download Ciphertext")
        self.save_cipher.clicked.connect(lambda: save_text_to_file(self.output_ciphertext.toPlainText(), "ciphertext.txt"))

        layout.addLayout(output_row("Key ID:", self.output_keyid, self.copy_keyid, self.save_keyid))
        layout.addLayout(output_row("IV:", self.output_iv, self.copy_iv, self.save_iv))

        # Ciphertext block
        layout.addLayout(inline_row("Ciphertext:", QLabel("")))
        cipher_block = QHBoxLayout()
        cipher_block.setContentsMargins(12, 0, 12, 10)
        cipher_block.addWidget(self.output_ciphertext)
        cipher_block.addWidget(self.copy_cipher)
        cipher_block.addWidget(self.save_cipher)

        layout.addLayout(cipher_block)

        container = QWidget()
        container.setLayout(layout)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(container)

        outer_layout = QVBoxLayout(self)
        outer_layout.addWidget(scroll)

    def upload_file(self):
        filename, _ = QFileDialog.getOpenFileName(self, "Open Plaintext File", "", "Text Files (*.txt)")
        if filename:
            with open(filename, 'r') as f:
                self.plaintext_input.setPlainText(f.read())

    def encrypt_data(self):
        try:
            option = KEY_OPTIONS[self.key_option.currentText()]
            key_state = KEY_STATES[self.key_state.currentText()]
            iv_state = IV_STATES[self.iv_state.currentText()]

            key_data_raw = self.key_input.toPlainText().strip().encode()
            iv_data_raw = self.iv_input.toPlainText().strip().encode()
            plaintext_raw = self.plaintext_input.toPlainText().strip().encode()

            key_data = key_data_raw.ljust(16, b'\x00')[:16] if key_state != 0xF2 else b'\x00' * 16
            iv_data = iv_data_raw.ljust(16, b'\x00')[:16] if iv_state != 0xE2 else b'\x00' * 16

            input_data = bytes([key_state]) + key_data + bytes([iv_state]) + iv_data + plaintext_raw
            input_size = len(input_data).to_bytes(2, 'big')
            packet, txid = build_packet(CMD_ENCRYPT, option, input_size + input_data)
            self.device.send(packet)

            response = self.device.receive()
            output_data = parse_packet(response, txid)

            key_id = output_data[:4]
            iv = output_data[4:20]
            ciphertext = output_data[20:]

            iv_store.add_entry(key_id, iv, ciphertext)

            self.output_keyid.setPlainText(key_id.hex(' ').upper())
            self.output_iv.setPlainText(iv.hex(' ').upper())
            self.output_ciphertext.setPlainText(ciphertext.hex(' ').upper())
            log("Encryption successful.")

        except Exception as e:
            log(f"Encryption failed: {e}")
            self.output_ciphertext.setPlainText("Error: " + str(e))

        except Exception as e:
            log(f"Encryption failed: {e}")
            self.output_ciphertext.setPlainText("Error: " + str(e))