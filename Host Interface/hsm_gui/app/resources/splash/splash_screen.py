import os
import random
from PySide6.QtCore import Qt, QTimer, Signal
from PySide6.QtGui import QFontDatabase, QFont
from PySide6.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QSizePolicy

class CorruptedDecrypt(QWidget):
    finished = Signal()  # Signal to emit when done

    def __init__(self):
        super().__init__()
        self.setStyleSheet("background-color: black;")
        self.resize(960, 640)

        # 🚩 Ensure correct path to font
        font_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "fonts", "Orbitron-Regular.ttf"))
        font_id = QFontDatabase.addApplicationFont(font_path)
        if font_id == -1:
            raise RuntimeError(f"Orbitron-Regular.ttf not found at: {font_path}")
        family = QFontDatabase.applicationFontFamilies(font_id)[0]
        
        # 📏 Set large font size
        self.font = QFont(family)
        self.font.setPointSize(96)  # High point size

        # 🔡 Target text animation setup
        self.target_text = "TrustX"
        self.displayed = [" "] * len(self.target_text)
        self.settled = [False] * len(self.target_text)
        self.char_pool = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%&*!?")

        # 🖼 Label setup
        self.label = QLabel("".join(self.displayed), self)
        self.label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.label.setFont(self.font)
        self.label.setStyleSheet("color: white;")
        self.label.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        # 📦 Layout setup
        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.label)
        self.setLayout(layout)

        # ⏱ Timer for glitch animation
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_text)
        self.ticks = 0

    def start(self):
        self.timer.start(50)

    def update_text(self):
        self.ticks += 1
        updated = False

        for i, char in enumerate(self.target_text):
            if self.settled[i]:
                continue
            if random.random() < 0.05 + self.ticks / 200:
                self.displayed[i] = char
                self.settled[i] = True
            else:
                self.displayed[i] = random.choice(self.char_pool)
            updated = True

        self.label.setText("".join(self.displayed))

        if not updated:
            self.timer.stop()
            QTimer.singleShot(400, self.finished.emit)
