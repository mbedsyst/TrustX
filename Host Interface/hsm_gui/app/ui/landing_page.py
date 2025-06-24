# landing_page.py
from PySide6.QtWidgets import QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QTextEdit
from app.core.logger import log
from app.core.device_comm import DeviceInterface
from app.ui.widgets.toast import Toast
from app.ui.widgets.clock import ClockWidget

class LandingPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.device = device

        self.clock = ClockWidget()

        self.connect_btn = QPushButton("Connect Device")
        self.connect_btn.clicked.connect(self.connect_device)

        self.disconnect_btn = QPushButton("Disconnect Device")
        self.disconnect_btn.clicked.connect(self.disconnect_device)

        layout = QVBoxLayout()
        top_layout = QHBoxLayout()
        top_layout.addStretch()
        top_layout.addWidget(self.clock)

        layout.addLayout(top_layout)
        layout.addWidget(self.connect_btn)
        layout.addWidget(self.disconnect_btn)
        self.setLayout(layout)

    def show_toast(self, message):
        Toast(self, message)

    def connect_device(self):
        try:
            if self.device.connect():
                log("Device Connected")
                self.show_toast("Device Connected")
        except Exception as e:
            log(f"Connection Failed: {str(e)}")
            self.show_toast(f"Connection Failed: {str(e)}")
    
    def disconnect_device(self):
        if self.device.disconnect():
            log("Device Disconnected.")
            self.show_toast("Device Disconnected.")
        else:
            log("No active Connection.")
            self.show_toast("No active Connection.")
