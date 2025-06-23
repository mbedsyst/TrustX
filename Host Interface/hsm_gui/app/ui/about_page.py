# app/ui/about_page.py

from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout

class AboutPage(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.addWidget(QLabel("About Page"))
        self.setLayout(layout)