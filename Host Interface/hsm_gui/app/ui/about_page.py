# app/ui/about_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PySide6.QtGui import QPixmap, QDesktopServices, QFont
from PySide6.QtCore import Qt, QUrl, QSize
import os

class AboutPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device
        layout = QVBoxLayout()
        layout.setAlignment(Qt.AlignTop)

        title = QLabel("Trust X")
        title.setFont(QFont("ARial", 24, QFont.Bold))
        title.setAlignment(Qt.AlignCenter)

        description = QLabel(
            "TrustX is an experimental Hardware Security Module developed for secure cryptographic operations "
            "using an STM32H5 microcontroller.This interface enables Key Generation, Encryption, Hashing, HMAC "
            "Generation, and Secure Random Number Generation — all handled securely within the hardware."
            "Developed with care to ensure privacy, modularity, and clarity for engineers and enthusiasts entering "
            "the field of embedded cryptography."
        )
        description.setWordWrap(True)

        info = QLabel(
            "Version: 1.0.0 (Beta)\n"
            "Firmware: STM32H563ZI v1.2.1\n"
            "License: MIT — Open for educational and research purposes.\n"
            "Developer: mbedsyst"
        )
        info.setWordWrap(True)

        tech = QLabel(            
            "Built With:\n"
            "- STM32CubeH5 HAL\n"
            "- Intel tinycrypt\n"
            "- PySide6 Python GUI\n"
        )
        tech.setWordWrap(True)

        disclaimer = QLabel(
            "⚠️ This tool is intended for educational and research purposes only. It is not certified for production use."
        )
        disclaimer.setWordWrap(True)
        disclaimer.setStyleSheet("color: red; font-weight: bold;")

        icon_row = QHBoxLayout()
        icon_row.setAlignment(Qt.AlignCenter)

        social_links = {
            "github.png": "https://github.com/mbedsyst",
            "blogger.png": "https://mbedsyst.blogspot.com/",
            "linkedin.png": "https://www.linkedin.com/in/zalmanulfarizpa/",
            "gmail.png": "mailto:zalmanulfarizpa@gmail.com",
            "instagram.png": "https://www.instagram.com/mbedsyst/",
        }

        icon_path = os.path.join(os.path.dirname(__file__), "..", "resources", "images")

        for icon_file, url in social_links.items():
            icon_button = QPushButton()
            icon_button.setIconSize(QSize(32, 32))
            icon_button.setFixedSize(40, 40)
            icon = QPixmap(os.path.join(icon_path, icon_file))
            icon_button.setIcon(icon)
            icon_button.setStyleSheet("border: none;")
            icon_button.clicked.connect(lambda _, link=url: QDesktopServices.openUrl(QUrl(link)))
            icon_row.addWidget(icon_button)

        layout.addWidget(title)
        layout.addSpacing(10)
        layout.addWidget(description)
        layout.addSpacing(10)
        layout.addWidget(info)
        layout.addSpacing(10)
        layout.addWidget(tech)
        layout.addSpacing(10)
        layout.addWidget(disclaimer)
        layout.addSpacing(10)
        layout.addLayout(icon_row)

        self.setLayout(layout)