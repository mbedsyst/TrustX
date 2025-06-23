# app/ui/otp_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class OTPPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("One-Time Pad Generation Page"))
        self.setLayout(layout)