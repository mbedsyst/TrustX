# app/ui/widgets/debug_terminal.py

from PySide6.QtWidgets import QTextEdit

class DebugTerminal(QTextEdit):
    def __init__(self):
        super().__init__()
        self.setReadOnly(True)
        self.setFixedHeight(150)
        self.setStyleSheet("font-family: monospace;")

    def append_message(self, message: str):
        self.append(message)