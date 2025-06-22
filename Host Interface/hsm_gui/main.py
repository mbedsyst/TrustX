# main.py
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QPushButton,
    QVBoxLayout, QMessageBox
)

import serial
import sys

class HSMGui(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HSM Interface")
        self.setGeometry(960, 540, 800, 400)
        
        # Holds the serail object
        self.serial_port = None

        #Central Widget and Layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout()
        central_widget.setLayout(layout)

        #Connect button
        self.connect_btn = QPushButton("Connect to Device")
        self.connect_btn.clicked.connect(self.connect_device)
        layout.addWidget(self.connect_btn)

        #Disconnect button
        self.disconnect_btn = QPushButton("Disconnect Device")
        self.disconnect_btn.clicked.connect(self.disconnect_device)
        layout.addWidget(self.disconnect_btn)

    def connect_device(self):
            try:
                self.serial_port = serial.Serial('COM4', 115200, timeout=1)
                QMessageBox.information(self, "Connected", "Connected to Device on COM4.")
            except serial.SerialException as e:
                QMessageBox.critical(self, "Connection Error", f"Failed to connect:\n{e}")


    def disconnect_device(self):
            if self.serial_port and self.serial_port.is_open:
                self.serial_port.close()
                QMessageBox.information(self, "Disconnected", "Disconnected from Device.")
            else:
                QMessageBox.warning(self, "Not Connected", "Device was not Connected")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = HSMGui()
    window.show()
    sys.exit(app.exec())

