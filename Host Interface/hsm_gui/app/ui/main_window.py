# app/ui/main_window.py

from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QStackedLayout, QStackedWidget
from PySide6.QtCore import QTimer
from app.core.logger import set_gui_log_callback
from app.core.device_comm import DeviceInterface
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
from app.resources.splash.splash_screen import CorruptedDecrypt


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("TrustX Console")
        self.setFixedSize(960, 640)

        self.device = DeviceInterface()
        self.debug_terminal = DebugTerminal()
        set_gui_log_callback(self.debug_terminal.append_message)

        # Central widget with stacked layout
        self.central = QWidget()
        self.setCentralWidget(self.central)

        self.stack = QStackedLayout()
        self.central.setLayout(self.stack)

        # Splash screen setup
        self.splash = CorruptedDecrypt()
        self.splash.finished.connect(self.init_main_gui)
        self.stack.addWidget(self.splash)
        self.splash.start()

        # Placeholder for main GUI
        self.main_gui = None

    def init_main_gui(self):
        # Build the main GUI
        gui_container = QWidget()
        gui_container.setObjectName("centralWidget")
        gui_container.setStyleSheet("background-color: #000000;")

        main_layout = QHBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        gui_container.setLayout(main_layout)

        self.sidebar = Sidebar()
        self.stack_widget = QStackedWidget()

        # Add application pages
        self.pages = {
            "landing": LandingPage(self.device),
            "encrypt": EncryptPage(self.device),
            "decrypt": DecryptPage(self.device),
            "hash": HashingPage(self.device),
            "hmac": HMACPage(self.device),
            "rng": RNGPage(self.device),
            "otp": OTPPage(self.device),
            "keygen": KeyGenPage(self.device),
            "keystore": KeyStorePage(self.device),
            "keydelete": KeyDeletePage(self.device),
            "about": AboutPage(self.device),
        }

        for key, page in self.pages.items():
            self.stack_widget.addWidget(page)

        self.sidebar.navigate.connect(self.switch_page)

        main_layout.addWidget(self.sidebar)
        main_layout.addWidget(self.stack_widget)

        SHOW_DEBUG_TERMINAL = False
        if SHOW_DEBUG_TERMINAL:
            main_layout.addWidget(self.debug_terminal)

        self.switch_page("landing")

        self.stack.addWidget(gui_container)
        self.stack.setCurrentWidget(gui_container)

    def switch_page(self, key):
        widget = self.pages.get(key)
        if widget:
            self.stack_widget.setCurrentWidget(widget)
