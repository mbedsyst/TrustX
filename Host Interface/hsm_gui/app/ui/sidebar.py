# app/ui/sidebar.py

from PySide6.QtWidgets import QWidget, QVBoxLayout, QPushButton, QSizePolicy
from PySide6.QtGui import QIcon
from PySide6.QtCore import Signal, QSize

class Sidebar(QWidget):
    # Emits Page Name
    navigate = Signal(str) 

    def __init__(self):
        super().__init__()
        self.setFixedWidth(200)
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        self.buttons = {}

        operations = [
            ("Home", "landing"),
            ("Encrypt", "encrypt"),
            ("Decrypt", "decrypt"),
            ("Hash", "hash"),
            ("HMAC", "hmac"),
            ("RNG", "rng"),
            ("OTP", "otp"),
            ("Key Gen", "keygen"),
            ("Key Store", "key store"),
            ("Key Delete", "keydelete"),
            ("About", "about"),
        ]

        for label, page_key in operations:
            btn = QPushButton(label)
            btn.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
            btn.clicked.connect(lambda checked, key=page_key: self.navigate.emit(key))
            self.layout.addWidget(btn)
            self.buttons[page_key] = btn

        self.layout.addStretch()    