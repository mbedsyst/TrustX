# app/ui/rng_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class RNGPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Random Number Generation Page"))
        self.setLayout(layout)