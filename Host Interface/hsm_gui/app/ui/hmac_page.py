# app/ui/hmac_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class HMACPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("HMAC Page"))
        self.setLayout(layout)