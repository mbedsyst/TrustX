# app/ui/sidebar.py

from PySide6.QtWidgets import QWidget, QVBoxLayout, QPushButton, QLabel, QSizePolicy
from PySide6.QtCore import Signal, Qt
from PySide6.QtGui import QPixmap

class Sidebar(QWidget):
    navigate = Signal(str)

    def __init__(self):
        super().__init__()
        self.setFixedWidth(200)
        self.setObjectName("sidebar")

        layout = QVBoxLayout()
        layout.setSpacing(0)
        layout.setContentsMargins(10, 0, 0, 10)
        self.setLayout(layout)

        # 🖼️ TX Monogram Logo at Top
        logo = QLabel()
        logo.setAlignment(Qt.AlignCenter)
        logo.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        pixmap = QPixmap("app/resources/icons/monogram_logo.png")
        scaled = pixmap.scaled(158, 110, Qt.KeepAspectRatio, Qt.SmoothTransformation)
        logo.setPixmap(scaled)
        logo.setFixedHeight(110)

        layout.addWidget(logo)

        # 📄 Sidebar Navigation Buttons
        operations = [
            ("Home", "landing"),
            ("Encrypt", "encrypt"),
            ("Decrypt", "decrypt"),
            ("Hash", "hash"),
            ("HMAC", "hmac"),
            ("Random Number", "rng"),
            ("One-Time Pad", "otp"),
            ("Key Generator", "keygen"),
            ("Key Store", "keystore"),
            ("Key Delete", "keydelete"),
            ("About", "about"),
        ]

        self.buttons = {}

        for label, page_key in operations:
            btn = QPushButton(label)
            btn.setObjectName("sidebarButton")
            btn.setCursor(Qt.PointingHandCursor)
            btn.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
            btn.setMinimumHeight(44)  # full-width rectangular buttons
            btn.clicked.connect(lambda _, k=page_key: self.navigate.emit(k))
            layout.addWidget(btn)
            self.buttons[page_key] = btn

        layout.addStretch()  # Push everything up and keep base contact
