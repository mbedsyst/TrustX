# app/ui/encrypt_page.py

from PySide6.QtWidgets import  (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton,
    QComboBox, QTextEdit, QFileDialog
)
from PySide6.QtWidgets import QScrollArea
from PySide6.QtCore import Qt
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.iv_store import IVStore
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log

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

class EncryptPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device
        
        self.key_option = QComboBox()
        self.key_option.addItems(KEY_OPTIONS.keys())

        self.key_state = QComboBox()
        self.key_state.addItems(KEY_STATES.keys())

        self.iv_state = QComboBox()
        self.iv_state.addItems(IV_STATES.keys())

        self.key_input = QTextEdit()
        self.key_input.setPlaceholderText("Enter 16-byte Key or 4-byte Key ID")
        self.key_input.setFixedHeight(40)

        self.iv_input = QTextEdit()
        self.iv_input.setPlaceholderText("Enter 16-byte Initialization Vector")
        self.iv_input.setFixedHeight(40)

        self.plaintext_input = QTextEdit()
        self.plaintext_input.setPlaceholderText("Enter Plaintext or Upload file")
        
        self.upload_btn = QPushButton("Upload Plaintext File")
        self.upload_btn.clicked.connect(self.upload_file)

        self.encrypt_btn = QPushButton("Encrypt")
        self.encrypt_btn.clicked.connect(self.encrypt_data)

        self.output_keyid = QTextEdit(); self.output_keyid.setReadOnly(True)
        self.output_iv = QTextEdit(); self.output_iv.setReadOnly(True)
        self.output_ciphertext = QTextEdit(); self.output_ciphertext.setReadOnly(True)

        self.copy_keyid = QPushButton("Copy KeyID")
        self.copy_keyid.clicked.connect(lambda: copy_to_clipboard(self.output_keyid.toPlainText()))
        self.save_keyid = QPushButton("Download KeyID")
        self.save_keyid.clicked.connect(lambda: save_text_to_file(self.output_keyid.toPlainText(), "key_id.txt"))

        self.copy_iv = QPushButton("Copy IV")
        self.copy_iv.clicked.connect(lambda: copy_to_clipboard(self.output_iv.toPlainText()))
        self.save_iv = QPushButton("Download IV")
        self.save_iv.clicked.connect(lambda: save_text_to_file(self.output_iv.toPlainText(), "iv.txt"))

        self.copy_cipher = QPushButton("Copy Ciphertext")
        self.copy_cipher.clicked.connect(lambda: copy_to_clipboard(self.output_ciphertext.toPlainText()))
        self.save_cipher = QPushButton("Download Ciphertext")
        self.save_cipher.clicked.connect(lambda: save_text_to_file(self.output_ciphertext.toPlainText(), "ciphertext.txt"))

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Key Option:"))
        layout.addWidget(self.key_option)
        layout.addWidget(QLabel("Key State:"))
        layout.addWidget(self.key_state)
        layout.addWidget(QLabel("Key Data / ID:"))
        layout.addWidget(self.key_input)
        layout.addWidget(QLabel("IV State:"))
        layout.addWidget(self.iv_state)
        layout.addWidget(QLabel("IV Data:"))
        layout.addWidget(self.iv_input)
        layout.addWidget(QLabel("Plaintext:"))
        layout.addWidget(self.plaintext_input)
        layout.addWidget(self.upload_btn)
        layout.addWidget(self.encrypt_btn)

        layout.addWidget(QLabel("Key ID:"))
        layout.addWidget(self.output_keyid)
        layout.addWidget(self.copy_keyid)
        layout.addWidget(self.save_keyid)

        layout.addWidget(QLabel("IV:"))
        layout.addWidget(self.output_iv)
        layout.addWidget(self.copy_iv)
        layout.addWidget(self.save_iv)

        layout.addWidget(QLabel("Ciphertext:"))
        layout.addWidget(self.output_ciphertext)
        layout.addWidget(self.copy_cipher)
        layout.addWidget(self.save_cipher)

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


