# app/utils/file_utils.py

from PySide6.QtWidgets import QFileDialog

def save_text_to_file(data: str, default_name="output.txt"):
    path, _ = QFileDialog.getSaveFileName(None, "Save File", default_name, "Text Files(*.txt)")
    if path:
        with open(path, "w") as f:
            f.write(data)