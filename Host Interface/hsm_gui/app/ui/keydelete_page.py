# app/ui/keydelete_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class KeyDeletePage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Key Delete Page"))
        self.setLayout(layout)