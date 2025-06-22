# landing_page.py
from PySide6.QtWidgets import QWidget, QPushButton, QVBoxLayout, QMessageBox
from app.core.device_comm import DeviceInterface

class LandingPage(QWidget):
    def __init__(self):
        super().__init__()

        self.device = DeviceInterface()

        self.connect_btn = QPushButton("Connect Device")
        self.connect_btn.clicked.connect(self.connect_device)

        self.disconnect_btn = QPushButton("Disconnect Device")
        self.disconnect_btn.clicked.connect(self.disconnect_device)

        layout = QVBoxLayout()
        layout.addWidget(self.connect_btn)
        layout.addWidget(self.disconnect_btn)
        self.setLayout(layout)

    def connect_device(self):
        try:
            if self.device.connect():
                QMessageBox.information(self, "Success", "Device Connected.")
        except Exception as e:
            QMessageBox.critical(self, "Connection Failed", str(e))
    
    def disconnect_device(self):
        if self.device.disconnect():
            QMessageBox.information(self, "Disconnected", "Device Disconnected.")
        else:
            QMessageBox.warning(self, "Warning", "No active Connection to Disconnect.")
