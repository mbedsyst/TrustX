# app/ui/decrypt_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton,
    QComboBox, QTextEdit, QFileDialog, QRadioButton, QButtonGroup,
    QScrollArea, QSizePolicy
)
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file, load_file_data
from app.core.logger import log
from app.core.iv_store import IVStore
import binascii

KEY_OPTIONS = {
    "128-bit Key": 0x11,
    "256-bit Key": 0x12
}

KEY_STATES = {
    "Provide Key (BYOK)": 0xD1,
    "Use Key ID (DABA)": 0xD2
}

IV_STATES = {
    "Provide IV (BYIV)": 0xC1,
    "Lookup IV (DABA)": 0xC2
}

class DecryptPage(QWidget):
    def __init__(self, device: DeviceInterface):
        super().__init__()
        self.device = device

        self.key_option = QComboBox()
        self.key_option.addItems(KEY_OPTIONS.keys())

        self.key_state = QComboBox()
        self.key_state.addItems(KEY_STATES.keys())

        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Key (hex or Key ID in hex)")
        self.key_input.setFixedHeight(40)

        self.iv_state = QComboBox()
        self.iv_state.addItems(IV_STATES.keys())

        self.iv_input = QTextEdit()
        self.iv_input.setPlaceholderText("IV (hex, required only if BYIV)")
        self.iv_input.setFixedHeight(40)

        self.ciphertext_input = QTextEdit()
        self.ciphertext_input.setPlaceholderText("Paste ciphertext or upload file")

        self.upload_btn = QPushButton("Upload File")
        self.upload_btn.clicked.connect(self.upload_file)

        self.decrypt_btn = QPushButton("Decrypt")
        self.decrypt_btn.clicked.connect(self.perform_decryption)

        # Output Fields
        self.output_keyid = QTextEdit(); self.output_keyid.setReadOnly(True); self.output_keyid.setFixedHeight(40)
        self.output_iv = QTextEdit(); self.output_iv.setReadOnly(True); self.output_iv.setFixedHeight(40)
        self.output_plaintext = QTextEdit(); self.output_plaintext.setReadOnly(True)

        self.copy_keyid_btn = QPushButton("Copy KeyID")
        self.copy_iv_btn = QPushButton("Copy IV")
        self.copy_plaintext_btn = QPushButton("Copy Plaintext")
        self.save_plaintext_btn = QPushButton("Download Plaintext")

        self.copy_keyid_btn.clicked.connect(lambda: copy_to_clipboard(self.output_keyid.toPlainText()))
        self.copy_iv_btn.clicked.connect(lambda: copy_to_clipboard(self.output_iv.toPlainText()))
        self.copy_plaintext_btn.clicked.connect(lambda: copy_to_clipboard(self.output_plaintext.toPlainText()))
        self.save_plaintext_btn.clicked.connect(lambda: save_text_to_file(self.output_plaintext.toPlainText(), "plaintext.txt"))

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Key Option:")); layout.addWidget(self.key_option)
        layout.addWidget(QLabel("Key State:")); layout.addWidget(self.key_state)
        layout.addWidget(QLabel("Key or Key ID:")); layout.addWidget(self.key_input)
        layout.addWidget(QLabel("IV State:")); layout.addWidget(self.iv_state)
        layout.addWidget(QLabel("IV (if applicable):")); layout.addWidget(self.iv_input)
        layout.addWidget(QLabel("Ciphertext:")); layout.addWidget(self.ciphertext_input)
        layout.addWidget(self.upload_btn); layout.addWidget(self.decrypt_btn)

        layout.addWidget(QLabel("Decrypted Key ID:")); layout.addWidget(self.output_keyid); layout.addWidget(self.copy_keyid_btn)
        layout.addWidget(QLabel("Decrypted IV:")); layout.addWidget(self.output_iv); layout.addWidget(self.copy_iv_btn)
        layout.addWidget(QLabel("Plaintext Output:")); layout.addWidget(self.output_plaintext)
        layout.addWidget(self.copy_plaintext_btn); layout.addWidget(self.save_plaintext_btn)

        container = QWidget(); container.setLayout(layout)
        scroll = QScrollArea(); scroll.setWidgetResizable(True); scroll.setWidget(container)
        outer_layout = QVBoxLayout(self); outer_layout.addWidget(scroll)

    def upload_file(self):
        content = load_file_data()
        if content:
            self.ciphertext_input.setPlainText(content)

    def perform_decryption(self):
        try:
            cmd = 0x02
            option = KEY_OPTIONS[self.key_option.currentText()]
            key_state = KEY_STATES[self.key_state.currentText()]
            iv_state = IV_STATES[self.iv_state.currentText()]

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