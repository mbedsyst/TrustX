# main_window.py
from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QStackedWidget
from app.core.logger import set_gui_log_callback
from app.ui.widgets.debug_terminal import DebugTerminal
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

        self.debug_terminal = DebugTerminal()
        set_gui_log_callback(self.debug_terminal.append_message)
        
        central = QWidget()
        main_layout = QVBoxLayout()
        central.setLayout(main_layout)
        self.setCentralWidget(central)

        page_layout = QHBoxLayout()
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

        page_layout.addWidget(self.sidebar)
        page_layout.addWidget(self.stack)

        main_layout.addLayout(page_layout)

        SHOW_DEBUG_TERMINAL = True
        if SHOW_DEBUG_TERMINAL:
            main_layout.addWidget(self.debug_terminal)

        self.switch_page("landing")

    def switch_page(self, key):
        widget = self.pages.get(key)
        if widget:
            self.stack.setCurrentWidget(widget)
        