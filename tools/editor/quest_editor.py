from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QListWidget, QPushButton, QLineEdit, QTextEdit, QLabel, QSpinBox
from property_editor import PropertyEditor

class QuestEditor(QWidget):
    def __init__(self):
        super().__init__()
        self.quests = []
        self.current_quest = None
        self.list_widget = QListWidget()
        self.add_btn = QPushButton("添加任务")
        self.remove_btn = QPushButton("删除任务")
        self.prop_editor = PropertyEditor()
        layout = QVBoxLayout()
        layout.addWidget(self.list_widget)
        hbox = QHBoxLayout()
        hbox.addWidget(self.add_btn)
        hbox.addWidget(self.remove_btn)
        layout.addLayout(hbox)
        layout.addWidget(QLabel("任务属性"))
        layout.addWidget(self.prop_editor)
        self.setLayout(layout)
        self.add_btn.clicked.connect(self.add_quest)
        self.remove_btn.clicked.connect(self.remove_quest)
        self.list_widget.currentRowChanged.connect(self.select_quest)

    def add_quest(self):
        q = {"id": f"quest_{len(self.quests)+1}", "title": "新任务", "description": "", "objectives": []}
        self.quests.append(q)
        self.list_widget.addItem(q['title'])
        self.list_widget.setCurrentRow(len(self.quests)-1)

    def remove_quest(self):
        idx = self.list_widget.currentRow()
        if idx >= 0:
            del self.quests[idx]
            self.list_widget.takeItem(idx)

    def select_quest(self, idx):
        if idx >= 0 and idx < len(self.quests):
            self.current_quest = self.quests[idx]
            schema = {
                "title": {"type":"str","label":"标题"},
                "description": {"type":"str","label":"描述"}
            }
            self.prop_editor.set_object(self.current_quest, schema)
