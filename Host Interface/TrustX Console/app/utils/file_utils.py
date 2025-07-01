# app/utils/file_utils.py

from PySide6.QtWidgets import QFileDialog

def save_text_to_file(data: str, default_name="output.txt"):
    path, _ = QFileDialog.getSaveFileName(None, "Save File", default_name, "Text Files(*.txt)")
    if path:
        with open(path, "w") as f:
            f.write(data)

def load_file_data(parent=None):
    file_path, _ = QFileDialog.getOpenFileName(parent, "Open File", "", "Text Files (*.txt);;All Files (*)")
    if file_path:
        with open(file_path, 'rb') as f:
            return f.read()
    return None