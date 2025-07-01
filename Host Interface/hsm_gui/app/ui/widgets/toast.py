# app/ui/widgets/toast.py

from PySide6.QtWidgets import QLabel, QGraphicsOpacityEffect
from PySide6.QtCore import Qt, QTimer, QPropertyAnimation

class Toast(QLabel):
    def __init__(self, parent, message, duration=2000):
        super().__init__(parent)

        self.setText(message)
        self.setStyleSheet("""
            background-color: #444;
            color: white;
            padding: 10px 20px;
            border-radius: 10px;
            font-size: 14px;
        """)
        self.setAlignment(Qt.AlignCenter)
        self.setWindowFlags(Qt.ToolTip)
        self.setAttribute(Qt.WA_TransparentForMouseEvents)

        self.adjustSize()
        self.move(
            (parent.width() - self.width()) //2,
            parent.height() - self.height() -50
        )

        #Opacity Effect
        opacity_effect = QGraphicsOpacityEffect(self)
        self.setGraphicsEffect(opacity_effect)
        self._fade = QPropertyAnimation(opacity_effect, b"opacity")
        self._fade.setDuration(500)
        self._fade.setStartValue(1.0)
        self._fade.setEndValue(0.0)

        QTimer.singleShot(duration, self.start_fade)

        self.show()

    def start_fade(self):
        self._fade.start()
        self._fade.finished.connect(self.deleteLater)