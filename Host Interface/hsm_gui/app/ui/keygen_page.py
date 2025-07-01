# app/ui/keygen_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout,
    QPushButton, QComboBox, QTextEdit, QSizePolicy
)
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QIcon, QFont, QPainter, QColor
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log
import os
import random

KEYGEN_OPTIONS = {
    "16 bytes": 0x43,
    "24 bytes": 0x44,
    "32 bytes": 0x45,
    "64 bytes": 0x47,
    "66 bytes": 0x48,
    "128 bytes": 0x49,
    "256 bytes": 0x4A,
    "384 bytes": 0x4B,
    "512 bytes": 0x4C,
}

ICON_PATH = "app/resources/icons"

class GridWidget(QWidget):
    def __init__(self, rows=15, cols=74, cell_size=10):
        super().__init__()
        self.rows = rows
        self.cols = cols
        self.cell_size = cell_size
        self.setFixedSize(self.cols * self.cell_size, self.rows * self.cell_size)
        self.grid = [[False for _ in range(self.cols)] for _ in range(self.rows)]
        self.randomize_grid()
        QTimer.singleShot(0, self.update)

    def randomize_grid(self):
        self.grid = [[random.choice([False, True]) for _ in range(self.cols)] for _ in range(self.rows)]

    def paintEvent(self, event):
        painter = QPainter(self)
        for row in range(self.rows):
            for col in range(self.cols):
                x = col * self.cell_size
                y = row * self.cell_size
                painter.setPen(QColor("white"))
                if self.grid[row][col]:
                    painter.setBrush(QColor("white"))
                else:
                    painter.setBrush(QColor("black"))
                painter.drawRect(x, y, self.cell_size, self.cell_size)

class KeyGenPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device

        layout = QVBoxLayout()

        self.grid_widget = GridWidget()
        layout.addWidget(self.grid_widget)

        self.dropdown = QComboBox()
        self.dropdown.addItem("Select Key Size")
        for label in KEYGEN_OPTIONS:
            self.dropdown.addItem(label)
        layout.addWidget(self.dropdown)

        self.generate_btn = QPushButton("Generate Key")
        self.generate_btn.clicked.connect(self.generate_key)
        layout.addWidget(self.generate_btn)

        layout.addWidget(QLabel("Generated Output (Hex):"))

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setFixedHeight(330)
        self.output_field.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.output_field.setFont(QFont("Courier New", 9))

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

    def generate_key(self):
        option_label = self.dropdown.currentText()
        if option_label not in KEYGEN_OPTIONS:
            self.output_field.setPlainText("Please select a valid Key size option.")
            return

        option = KEYGEN_OPTIONS[option_label]
        cmd = 0x07
        input_data = b'\x00\x00'

        try:
            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            hex_data = " ".join(f"{b:02X}" for b in output_data)
            self.output_field.setPlainText(hex_data)
            log(f"KEYGEN Response: {hex_data}")
        except Exception as e:
            log(f"KEYGEN Error: {str(e)}")
            self.output_field.setPlainText("Error: " + str(e))

    def copy_output(self):
        text = self.output_field.toPlainText()
        copy_to_clipboard(text)

    def save_output(self):
        text = self.output_field.toPlainText()
        save_text_to_file(text, default_name="key_generated.txt")
