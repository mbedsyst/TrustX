# app/ui/widgets/clock.py

from PySide6.QtWidgets import QLabel
from PySide6.QtCore import QTimer, QTime, Qt

class ClockWidget(QLabel):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.setObjectName("ClockWidget")  # Add an object name for QSS targeting

        timer = QTimer(self)
        timer.timeout.connect(self.update_time)
        timer.start(1000)

        self.update_time()

    def update_time(self):
        current_time = QTime.currentTime().toString("HH:mm:ss")
        self.setText(f"{current_time}")
