from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QListWidget, QPushButton, QTextEdit, QLineEdit, QLabel
from .property_editor import PropertyEditor

class EventEditor(QWidget):
    def __init__(self):
        super().__init__()
        self.events = []
        self.list_widget = QListWidget()
        self.add_btn = QPushButton("添加事件")
        self.remove_btn = QPushButton("删除事件")
        self.prop = PropertyEditor()
        layout = QVBoxLayout()
        layout.addWidget(self.list_widget)
        hbox = QHBoxLayout()
        hbox.addWidget(self.add_btn)
        hbox.addWidget(self.remove_btn)
        layout.addLayout(hbox)
        layout.addWidget(self.prop)
        self.setLayout(layout)
        self.add_btn.clicked.connect(self.add_event)
        self.remove_btn.clicked.connect(self.remove_event)
        self.list_widget.currentRowChanged.connect(self.select_event)

    def add_event(self):
        ev = {"name": "新事件", "trigger": "auto", "action": ""}
        self.events.append(ev)
        self.list_widget.addItem(ev['name'])
        self.list_widget.setCurrentRow(len(self.events)-1)

    def remove_event(self):
        idx = self.list_widget.currentRow()
        if idx >= 0:
            del self.events[idx]
            self.list_widget.takeItem(idx)

    def select_event(self, idx):
        if idx >= 0 and idx < len(self.events):
            schema = {"name": {"type":"str","label":"名称"}, "trigger": {"type":"combo","label":"触发方式","options":["auto","interact","enter"]}, "action": {"type":"str","label":"Lua代码"}}
            self.prop.set_object(self.events[idx], schema)
