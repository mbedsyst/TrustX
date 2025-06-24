# app/ui/hashing_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout,
    QPushButton, QComboBox, QTextEdit, QFileDialog, QScrollArea
)
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log

HASH_OPTIONS = {
    "SHA-224": 0x21,
    "SHA-256": 0x22,
    "SHA-384": 0x23,
    "SHA-512": 0x24,
}

class HashingPage(QWidget):
    def __init__(self, device: DeviceInterface):
        super().__init__()
        self.device = device
        self.plaintext = b""

        self.dropdown = QComboBox()
        for label in HASH_OPTIONS:
            self.dropdown.addItem(label)

        self.input_field = QTextEdit()
        self.input_field.setPlaceholderText("Enter text or upload file...")

        self.upload_btn = QPushButton("Upload File")
        self.upload_btn.clicked.connect(self.upload_file)

        self.hash_btn = QPushButton("Generate Hash")
        self.hash_btn.clicked.connect(self.generate_hash)

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
        layout.addWidget(QLabel("Select Hash Algorithm:"))
        layout.addWidget(self.dropdown)
        layout.addWidget(QLabel("Input Data:"))
        layout.addWidget(self.input_field)
        layout.addWidget(self.upload_btn)
        layout.addWidget(self.hash_btn)
        layout.addWidget(QLabel("Digest (Hex):"))
        layout.addWidget(self.output_field)
        layout.addLayout(btn_layout)

        container = QWidget()
        container.setLayout(layout)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(container)

        outer = QVBoxLayout(self)
        outer.addWidget(scroll)

    def upload_file(self):
        file_path, _ = QFileDialog.getOpenFileName(self, "Open File", "", "All Files (*)")
        if file_path:
            with open(file_path, 'rb') as f:
                self.plaintext = f.read()
            self.input_field.setPlainText(self.plaintext.decode(errors='ignore'))

    def generate_hash(self):
        cmd = 0x03
        option = HASH_OPTIONS[self.dropdown.currentText()]
        text = self.input_field.toPlainText()
        self.plaintext = text.encode()

        input_size = len(self.plaintext).to_bytes(2, 'big')
        try:
            packet, txid = build_packet(cmd, option, input_size + self.plaintext)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            hex_digest = " ".join(f"{b:02X}" for b in output_data)
            self.output_field.setPlainText(hex_digest)
            log(f"Hash Response: {hex_digest}")
        except Exception as e:
            log(f"Hashing Error: {str(e)}")
            self.output_field.setPlainText("Error: " + str(e))

    def copy_output(self):
        text = self.output_field.toPlainText()
        copy_to_clipboard(text)

    def save_output(self):
        text = self.output_field.toPlainText()
        save_text_to_file(text, default_name="digest.txt")
