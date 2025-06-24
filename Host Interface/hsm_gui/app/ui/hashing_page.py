# app/ui/hashing_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class HashingPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Hashing Page"))
        self.setLayout(layout)