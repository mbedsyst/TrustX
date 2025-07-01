from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QFontDatabase, QIcon
from app.ui.main_window import MainWindow
import sys
import os

if __name__ == "__main__":
    app = QApplication(sys.argv)

    # Load Outfit font
    QFontDatabase.addApplicationFont("app/resources/fonts/Outfit-Regular.ttf")
    QFontDatabase.addApplicationFont("app/resources/fonts/Orbitron-Regular.ttf")

        # Set app icon
    icon_path = os.path.join(os.path.dirname(__file__), "app", "resources", "icons", "trustx_icon.png")
    app.setWindowIcon(QIcon(icon_path))  # ✅ This sets the taskbar and window icon

    # Apply stylesheet
    with open("app/resources/style.qss", "r") as f:
        style = f.read()
        app.setStyleSheet(style)

    window = MainWindow()
    window.show()

    sys.exit(app.exec())


