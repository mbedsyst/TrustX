# app/ui/encrypt_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class EncryptPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Encryption Page"))
        self.setLayout(layout)