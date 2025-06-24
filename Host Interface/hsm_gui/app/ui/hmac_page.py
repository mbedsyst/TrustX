# app/ui/hmac_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class HMACPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device
        layout = QVBoxLayout()
        layout.addWidget(QLabel("HMAC Page"))
        self.setLayout(layout)