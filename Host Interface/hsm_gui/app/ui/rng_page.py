# app/ui/rng_page.py

from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout,
    QPushButton, QComboBox, QTextEdit, QSizePolicy
)
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QColor, QPainter, QIcon
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log
import random
import os

RNG_OPTIONS = {
    "4 bytes": 0x41,
    "8 bytes": 0x42,
    "16 bytes": 0x43,
    "32 bytes": 0x45,
    "64 bytes": 0x47,
    "128 bytes": 0x49,
    "256 bytes": 0x4A,
    "512 bytes": 0x4C,
    "1024 bytes": 0x4D,
    "2048 bytes": 0x4E,
    "4096 bytes": 0x4F,
}

ICON_PATH = "app/resources/icons"

class GridWidget(QWidget):
    def __init__(self, rows=15, cols=60, cell_size=10):
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

class RNGPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device

        layout = QVBoxLayout()

        self.grid_visual = GridWidget(rows=15, cols=73, cell_size=10)
        layout.addWidget(self.grid_visual, alignment=Qt.AlignHCenter)

        self.dropdown = QComboBox()
        self.dropdown.addItem("Select Random Number Size")
        for label in RNG_OPTIONS:
            self.dropdown.addItem(label)
        layout.addWidget(self.dropdown)

        self.generate_btn = QPushButton("Generate Random Number")
        self.generate_btn.clicked.connect(self.generate_random)
        layout.addWidget(self.generate_btn)

        layout.addWidget(QLabel("Generated Output (Hex):"))

        self.output_field = QTextEdit()
        self.output_field.setReadOnly(True)
        self.output_field.setFixedHeight(330)
        self.output_field.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)

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

    def generate_random(self):
        option_label = self.dropdown.currentText()
        if option_label not in RNG_OPTIONS:
            self.output_field.setPlainText("Please select a valid RNG size option.")
            return

        option = RNG_OPTIONS[option_label]
        cmd = 0x05
        input_data = b'\x00\x00'

        try:
            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            hex_data = " ".join(f"{b:02X}" for b in output_data)
            self.output_field.setPlainText(hex_data)
            log(f"RNG Response: {hex_data}")
        except Exception as e:
            log(f"RNG Error: {str(e)}")
            self.output_field.setPlainText("Error: " + str(e))

    def copy_output(self):
        text = self.output_field.toPlainText()
        copy_to_clipboard(text)

    def save_output(self):
        text = self.output_field.toPlainText()
        save_text_to_file(text, default_name="random_number.txt")





"""from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QComboBox, QTextEdit
from app.core.packet_builder import build_packet
from app.core.packet_parser import parse_packet
from app.core.device_comm import DeviceInterface
from app.utils.clipboard_utils import copy_to_clipboard
from app.utils.file_utils import save_text_to_file
from app.core.logger import log

RNG_OPTIONS = {
    "4 bytes": 0x41,
    "8 bytes": 0x42,
    "16 bytes": 0x43,
    "32 bytes": 0x45,
    "64 bytes": 0x47,
    "128 bytes": 0x49,
    "256 bytes": 0x4A,
    "512 bytes": 0x4C,
    "1024 bytes": 0x4D,
    "2048 bytes": 0x4E,
    "4096 bytes": 0x4F,
}

class RNGPage(QWidget):
    def __init__(self, device):
        super().__init__()
        
        self.device = device

        self.dropdown = QComboBox()
        for label in RNG_OPTIONS:
            self.dropdown.addItem(label)

        self.generate_btn = QPushButton("Generate Random Number")
        self.generate_btn.clicked.connect(self.generate_random)

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
        layout.addWidget(QLabel("Select Random Number Size:"))
        layout.addWidget(self.dropdown)
        layout.addWidget(self.generate_btn)
        layout.addWidget(QLabel("Generated Output (Hex):"))
        layout.addWidget(self.output_field)
        layout.addLayout(btn_layout)
        self.setLayout(layout)

    def generate_random(self):
        option_label = self.dropdown.currentText()
        option = RNG_OPTIONS[option_label]

        cmd = 0x05
        input_data = b'\x00\x00'

        try:
            packet, txid = build_packet(cmd, option, input_data)
            self.device.send(packet)
            response = self.device.receive()
            output_data = parse_packet(response, txid)

            hex_data = " ".join(f"{b:02X}" for b in output_data)
            self.output_field.setPlainText(hex_data)
            log(f"RNG Response: {hex_data}")
        except Exception as e:
            log(f"RNG Error: {str(e)}")
            self.output_field.setPlainText("Error: " + str(e))
    
    def copy_output(self):
        text = self.output_field.toPlainText()
        copy_to_clipboard(text)

    def save_output(self):
        text = self.output_field.toPlainText()
        save_text_to_file(text, default_name="random_number.txt")"""
