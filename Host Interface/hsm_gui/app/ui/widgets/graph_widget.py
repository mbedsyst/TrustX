# app/ui/widgets/graph_widget.py

from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtCore import Qt, QUrl
from PySide6.QtWidgets import QSizePolicy  # ✅ Add this import
from PySide6.QtWebEngineCore import QWebEngineProfile, QWebEngineSettings
import os

class GraphWidget(QWebEngineView):
    def __init__(self, parent=None):
        super().__init__(parent)

        # ✅ Set transparency attributes
        self.setAttribute(Qt.WA_TranslucentBackground, True)
        self.setStyleSheet("background: transparent; border: none;")
        self.setContentsMargins(0, 0, 0, 0)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        # ✅ Set background color of the QWebEnginePage to transparent
        self.page().setBackgroundColor(Qt.transparent)

        # ✅ Load the HTML
        html_path = os.path.abspath("app/resources/animation/graph.html")
        self.load(QUrl.fromLocalFile(html_path))
