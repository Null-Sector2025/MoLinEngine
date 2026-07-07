import json, os
from PyQt5.QtCore import Qt, QRectF
from PyQt5.QtGui import QColor, QPainter, QFont
from PyQt5.QtWidgets import QMessageBox

def load_json(path):
    if not os.path.exists(path): return {}
    with open(path, 'r', encoding='utf-8') as f:
        return json.load(f)

def save_json(path, data):
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)

def show_message(parent, title, text, icon=QMessageBox.Information):
    msg = QMessageBox(parent)
    msg.setIcon(icon)
    msg.setWindowTitle(title)
    msg.setText(text)
    msg.exec_()
