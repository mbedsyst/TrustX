# main_window.py
from PySide6.QtWidgets import QMainWindow, QWidget, QHBoxLayout, QStackedWidget
from app.ui.landing_page import LandingPage
from app.ui.sidebar import Sidebar
from app.ui.encrypt_page import EncryptPage
from app.ui.decrypt_page import DecryptPage
from app.ui.hashing_page import HashingPage
from app.ui.hmac_page import HMACPage
from app.ui.rng_page import RNGPage
from app.ui.otp_page import OTPPage
from app.ui.keygen_page import KeyGenPage
from app.ui.keystore_page import KeyStorePage
from app.ui.keydelete_page import KeyDeletePage
from app.ui.about_page import AboutPage

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("TrustX Console")
        self.setMinimumSize(720, 480)

        central = QWidget()
        layout = QHBoxLayout()
        central.setLayout(layout)
        self.setCentralWidget(central)

        self.sidebar = Sidebar()
        self.stack = QStackedWidget()

        self.pages = {
            "landing": LandingPage(),
            "encrypt": EncryptPage(),
            "decrypt": DecryptPage(),
            "hashing": HashingPage(),
            "hmac": HMACPage(),
            "rng": RNGPage(),
            "otp": OTPPage(),
            "keygen": KeyGenPage(),
            "keystore": KeyStorePage(),
            "keydelete": KeyDeletePage(),
            "about": AboutPage(),
        }

        for key, page in self.pages.items():
            self.stack.addWidget(page)

        self.sidebar.navigate.connect(self.switch_page)

        layout.addWidget(self.sidebar)
        layout.addWidget(self.stack)

        self.switch_page("landing")

    def switch_page(self, key):
        widget = self.pages.get(key)
        if widget:
            self.stack.setCurrentWidget(widget)
        