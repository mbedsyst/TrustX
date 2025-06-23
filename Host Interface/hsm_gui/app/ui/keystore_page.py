# app/ui/keystore_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class KeyStorePage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Key Store Page"))
        self.setLayout(layout)