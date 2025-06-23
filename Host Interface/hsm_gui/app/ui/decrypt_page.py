# app/ui/decrypt_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class DecryptPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Decryption Page"))
        self.setLayout(layout)