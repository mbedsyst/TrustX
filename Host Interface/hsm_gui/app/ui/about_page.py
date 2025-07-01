# app/ui/about_page.py
# app/ui/about_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton, QSpacerItem, QSizePolicy
from PySide6.QtGui import QPixmap, QDesktopServices, QFont, QIcon
from PySide6.QtCore import Qt, QUrl, QSize
import os

class AboutPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device

        layout = QVBoxLayout()
        layout.setContentsMargins(40, 30, 40, 10)
        layout.setSpacing(15)

        # 🔠 Title
        title = QLabel("TrustX")
        title.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        title.setAlignment(Qt.AlignCenter)
        title.setFont(QFont("Orbitron", 200, QFont.Normal))
        title.setStyleSheet("font-family: Orbitron; font-size: 96px; color: white;")

        description = QLabel("""
            <div style="color: #ddd; font-size: 16px; text-align: justify;">
                TrustX is a standalone Hardware Security Module built around an STMicroelectronics STM32H5. It enables
                secure, offline cryptographic operations where sensitive keys and data never leave the hardware. This interface 
                was developed for experimenting with Embedded Cryptography, Secure Key Storage, and Real-World HSM design.
            </div>
        """)
        description.setWordWrap(True)  # Still needed
        description.setTextFormat(Qt.TextFormat.RichText)


        """# 📄 Description
        description = QLabel(
            "TrustX is a standalone Hardware Security Module built around the STMicroelectronics STM32H563ZI. It enables"
            "secure, offline cryptographic operations where sensitive keys and data never leave the hardware. This interface "
            "was developed for experimenting with Embedded Cryptography, Secure Key Storage, and Real-World HSM design."
        )
        description.setWordWrap(True)
        description.setStyleSheet("color: #ddd; font-size: 16px;")"""

        # ℹ️ Info
        info = QLabel(
            "Features\n"
            "- Intel TinyCrypt Symmetric Encryption/Decryption\n"
            "- FIPS-compliant hardware HASH Engine with MD5/SHA1/SHA2\n"
            "- TNIST SP 800-90B compliant TRNG Entropy source\n"
            "- 96-bit UID + 160-bit Runtime Salt for Key Derivation\n"
            "- OTP Generator for secure exchange\n"
            "- External Flash encrypted using MCU-derived key\n"
            "- USB Serial (CDC) Host Communication"
        )
        info.setWordWrap(True)
        info.setStyleSheet("color: #ddd; font-size: 14px;")

        # 🔧 Tech Stack
        tech = QLabel(
            "Trust X Firmware Version: 1.2.0 (Beta)\n"
            "Trust X Console Version: 1.0.0 (Beta)"
        )
        tech.setWordWrap(True)
        tech.setStyleSheet("color: #ddd; font-size: 14px;")

        # ⚠️ Disclaimer
        disclaimer = QLabel(
            "ⓘ  This tool is intended for educational and research purposes only. It is not certified for production use."
        )
        disclaimer.setWordWrap(True)
        disclaimer.setStyleSheet("color: #ddd; font-size: 14px;")
        
        # 🌐 Social Icons (with spacing + base alignment)
        icon_row = QHBoxLayout()
        icon_row.setSpacing(40)
        icon_row.setAlignment(Qt.AlignCenter)

        social_links = {
            "blogger.png": "https://mbedsyst.blogspot.com/",
            "github.png": "https://github.com/mbedsyst",
            "instagram.png": "https://www.instagram.com/mbedsyst/",
            "linkedin.png": "https://www.linkedin.com/in/zalmanulfarizpa/",
            "gmail.png": "mailto:zalmanulfarizpa@gmail.com",
            
        }

        icon_path = os.path.join(os.path.dirname(__file__), "..", "resources", "images")

        for icon_file, url in social_links.items():
            icon_button = QPushButton()
            icon_button.setIcon(QIcon(os.path.join(icon_path, icon_file)))
            icon_button.setIconSize(QSize(32, 32))
            icon_button.setFixedSize(40, 40)
            icon_button.setStyleSheet("border: none; background: none;")
            icon_button.clicked.connect(lambda _, link=url: QDesktopServices.openUrl(QUrl(link)))
            icon_row.addWidget(icon_button)

        # 📦 Assemble layout
        layout.addWidget(title)
        layout.addSpacing(20)
        layout.addWidget(description)
        layout.addSpacing(10)
        layout.addWidget(info)
        layout.addSpacing(10)
        layout.addWidget(tech)
        layout.addSpacing(10)
        layout.addWidget(disclaimer)
        layout.addStretch()  # Push icons to bottom
        layout.addLayout(icon_row)
        layout.addSpacing(10)  # 10px from base

        self.setLayout(layout)
        self.setStyleSheet("background-color: #000;")