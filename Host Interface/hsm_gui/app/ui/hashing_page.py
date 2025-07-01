# app/ui/hashing_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout,
    QPushButton, QComboBox, QTextEdit, QFileDialog, QScrollArea
)
from PySide6.QtCore import Qt
from PySide6.QtGui import QIcon
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log
import os

HASH_OPTIONS = {
    "SHA-224": 0x21,
    "SHA-256": 0x22,
    "SHA-384": 0x23,
    "SHA-512": 0x24,
}

ICON_PATH = os.path.join(os.path.dirname(__file__), "..", "resources", "icons")

class HashingPage(QWidget):
    def __init__(self, device: DeviceInterface):
        super().__init__()
        self.device = device
        self.plaintext = b""

        layout = QVBoxLayout()

        def inline_row(label_text, widget):
            row = QHBoxLayout()
            row.setContentsMargins(12, 0, 12, 0)
            label = QLabel(label_text)
            label.setFixedWidth(140)
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

        self.dropdown = QComboBox()
        self.dropdown.addItem("Select Algorithm")
        self.dropdown.addItems(HASH_OPTIONS.keys())

        self.input_field = QTextEdit()
        self.input_field.setPlaceholderText("Enter Text or Upload File")

        self.upload_btn = make_icon_button("upload", "Upload File")
        self.upload_btn.clicked.connect(self.upload_file)

        self.hash_btn = QPushButton("Generate Hash")
        self.hash_btn.clicked.connect(self.generate_hash)

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setFixedHeight(60)
        self.output_field.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.copy_btn = make_icon_button("copy.png", "Copy Digest")
        self.copy_btn.clicked.connect(self.copy_output)
        self.save_btn = make_icon_button("download.png", "Download Digest")
        self.save_btn.clicked.connect(self.save_output)

        layout.addLayout(inline_row("Hash Algorithm:", self.dropdown))

        input_row = QHBoxLayout()
        input_row.setContentsMargins(12, 0, 12, 0)
        label_input = QLabel("Input Data:")
        label_input.setFixedWidth(140)
        input_row.addWidget(label_input)
        input_row.addWidget(self.upload_btn)
        input_row.addStretch()

        layout.addLayout(input_row)
        layout.addWidget(self.input_field)
        layout.addWidget(self.hash_btn)

        digest_label_row = QHBoxLayout()
        digest_label_row.setContentsMargins(12, 0, 12, 0)
        digest_label = QLabel("Digest (Hex):")
        digest_label.setFixedWidth(140)
        digest_label_row.addWidget(digest_label)
        digest_label_row.addStretch()
        layout.addLayout(digest_label_row)

        digest_block = QHBoxLayout()
        digest_block.setContentsMargins(12, 0, 12, 10)
        digest_block.addWidget(self.output_field)
        digest_block.addWidget(self.copy_btn)
        digest_block.addWidget(self.save_btn)

        layout.addLayout(digest_block)

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