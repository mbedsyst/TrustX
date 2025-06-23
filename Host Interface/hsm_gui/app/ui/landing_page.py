# landing_page.py
from PySide6.QtWidgets import QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QTextEdit
from app.core.device_comm import DeviceInterface
from app.ui.widgets.toast import Toast
from app.core.logger import set_gui_log_callback
from app.ui.widgets.clock import ClockWidget

SHOW_DEBUG_TERMINAL = True

class LandingPage(QWidget):
    def __init__(self):
        super().__init__()

        self.device = DeviceInterface()

        self.clock = ClockWidget()

        self.connect_btn = QPushButton("Connect Device")
        self.connect_btn.clicked.connect(self.connect_device)

        self.disconnect_btn = QPushButton("Disconnect Device")
        self.disconnect_btn.clicked.connect(self.disconnect_device)

        # Debug Terminal, only for development
        self.debug_terminal = QTextEdit()
        self.debug_terminal.setReadOnly(True)
        self.debug_terminal.setStyleSheet("font-family: monospace;")
        self.debug_terminal.setFixedHeight(150)

        set_gui_log_callback(self.append_log)

        layout = QVBoxLayout()
        top_layout = QHBoxLayout()
        top_layout.addStretch()
        top_layout.addWidget(self.clock)

        layout.addLayout(top_layout)
        layout.addWidget(self.connect_btn)
        layout.addWidget(self.disconnect_btn)
        if SHOW_DEBUG_TERMINAL:
            layout.addWidget(self.debug_terminal)
        self.setLayout(layout)

    def append_log(self, message):
        self.debug_terminal.append(message)

    def show_toast(self, message):
        Toast(self, message)

    def connect_device(self):
        try:
            if self.device.connect():
                self.append_log("Device Connected")
                self.show_toast("Device Connected")
        except Exception as e:
            self.append_log(f"Connection Failed: {str(e)}")
            self.show_toast(f"Connection Failed: {str(e)}")
    
    def disconnect_device(self):
        if self.device.disconnect():
            self.append_log("Device Disconnected.")
            self.show_toast("Device Disconnected.")
        else:
            self.append_log("No active Connection.")
            self.show_toast("No active Connection.")
