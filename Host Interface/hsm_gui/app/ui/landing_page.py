# app/ui/landing_page.py

from PySide6.QtWidgets import (
    QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QSpacerItem, QSizePolicy
)
from PySide6.QtGui import QFont, Qt
from app.core.logger import log
from app.core.device_comm import DeviceInterface
from app.ui.widgets.toast import Toast
from app.ui.widgets.clock import ClockWidget
from app.ui.widgets.graph_widget import GraphWidget


class LandingPage(QWidget):
    def __init__(self, device):
        super().__init__()
        self.setObjectName("LandingPage")
        self.device = device

        # ⏰ Clock
        self.clock = ClockWidget()

        # 🕸️ Animation
        self.graph = GraphWidget()
        self.graph.setStyleSheet("background-color: black; border: none;")

        # 🔘 Buttons
        self.connect_btn = QPushButton("Connect")
        self.disconnect_btn = QPushButton("Disconnect")

        self.connect_btn.setFixedSize(120, 40)
        self.disconnect_btn.setFixedSize(120, 40)
        self.connect_btn.setObjectName("connectButton")
        self.disconnect_btn.setObjectName("disconnectButton")

        self.connect_btn.clicked.connect(self.connect_device)
        self.disconnect_btn.clicked.connect(self.disconnect_device)

        # 🔼 Top: Clock aligned to the top-right
        clock_layout = QHBoxLayout()
        clock_layout.addStretch()
        clock_layout.addWidget(self.clock)

        # 🔽 Bottom: Buttons aligned and pushed slightly left
        button_layout = QHBoxLayout()
        button_layout.addSpacerItem(QSpacerItem(160, 0, QSizePolicy.Fixed, QSizePolicy.Minimum))
        button_layout.addWidget(self.connect_btn)
        button_layout.addSpacing(60)
        button_layout.addWidget(self.disconnect_btn)
        button_layout.addStretch()
        button_layout.addSpacerItem(QSpacerItem(40, 0, QSizePolicy.Fixed, QSizePolicy.Minimum))

        # 🧱 Main Layout
        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        # 📐 Clock Layout at top-right
        clock_layout = QHBoxLayout()
        clock_layout.addStretch()
        clock_layout.addWidget(self.clock)
        clock_layout.setContentsMargins(10, 10, 10, 0)

        # 🕸️ Graph Widget (takes full space)
        self.graph.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        # 🔘 Button Layout
        button_layout = QHBoxLayout()
        button_layout.addSpacing(100)
        button_layout.addWidget(self.connect_btn)
        button_layout.addSpacing(60)
        button_layout.addWidget(self.disconnect_btn)
        button_layout.addStretch()
        button_layout.setContentsMargins(100, 10, 20, 20)

        # 📦 Stack everything
        main_layout.addLayout(clock_layout)
        main_layout.addWidget(self.graph, stretch=1)
        main_layout.addLayout(button_layout)

        self.setLayout(main_layout)
        self.setStyleSheet("background-color: #000000;")

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