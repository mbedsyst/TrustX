# app/ui/widgets/toast.py

from PySide6.QtWidgets import QLabel, QGraphicsOpacityEffect
from PySide6.QtCore import Qt, QTimer, QPropertyAnimation
from PySide6.QtGui import QFontDatabase, QFont

class Toast(QLabel):
    def __init__(self, parent, message, duration=2000):
        super().__init__(parent)

        # ✅ Load Orbitron font (bold)
        font_path = "app/resources/fonts/Orbitron-Regular.ttf"
        font_id = QFontDatabase.addApplicationFont(font_path)
        font_family = QFontDatabase.applicationFontFamilies(font_id)[0]
        orbitron = QFont(font_family)
        orbitron.setPointSize(12)
        orbitron.setBold(True)  # ✅ Make it bold

        self.setFont(orbitron)
        self.setText(message)
        self.setStyleSheet("""
            background-color: white;
            color: black;
            padding: 10px 20px;
            border-radius: 1px;
            font-size: 14px;
        """)
        self.setAlignment(Qt.AlignCenter)
        self.setWindowFlags(Qt.ToolTip)
        self.setAttribute(Qt.WA_TransparentForMouseEvents)

        self.adjustSize()

        # 🎯 Center horizontally and offset 100px from top
        self.move(
            600,  # Center horizontally
            150  # Offset downward by 100px
        )

        # 🎞 Fade-out animation
        opacity_effect = QGraphicsOpacityEffect(self)
        self.setGraphicsEffect(opacity_effect)
        self._fade = QPropertyAnimation(opacity_effect, b"opacity")
        self._fade.setDuration(300)
        self._fade.setStartValue(1.0)
        self._fade.setEndValue(0.0)

        QTimer.singleShot(duration, self.start_fade)
        self.show()

    def start_fade(self):
        self._fade.start()
        self._fade.finished.connect(self.deleteLater)
