# app/ui/widgets/graph_widget.py

from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtCore import Qt
from PySide6.QtWidgets import QSizePolicy  # ✅ Add this import
import os

class GraphWidget(QWebEngineView):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setStyleSheet("background: transparent; border: none;")
        self.setContentsMargins(0, 0, 0, 0)

        # ✅ Correct use of QSizePolicy class
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        html_path = os.path.abspath("app/resources/animation/graph.html")
        self.load(f"file:///{html_path.replace(os.sep, '/')}")
