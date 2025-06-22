# main_window.py
from PySide6.QtWidgets import QMainWindow, QStackedWidget
from app.ui.landing_page import LandingPage

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HSM GUI")
        self.setMinimumSize(600, 400)

        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)

        self.landing_page = LandingPage()
        self.stack.addWidget(self.landing_page)

        self.stack.setCurrentWidget(self.landing_page)