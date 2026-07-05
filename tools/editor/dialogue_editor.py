from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QTreeWidget, QTreeWidgetItem, QPushButton, QTextEdit, QLineEdit, QLabel
from PyQt5.QtCore import pyqtSignal

class DialogueEditor(QWidget):
    def __init__(self):
        super().__init__()
        self.tree = QTreeWidget()
        self.tree.setHeaderLabels(["对话节点"])
        self.add_btn = QPushButton("添加节点")
        self.remove_btn = QPushButton("删除节点")
        self.speaker_edit = QLineEdit()
        self.text_edit = QTextEdit()
        self.save_btn = QPushButton("保存对话")
        layout = QVBoxLayout()
        layout.addWidget(self.tree)
        hbox = QHBoxLayout()
        hbox.addWidget(self.add_btn)
        hbox.addWidget(self.remove_btn)
        layout.addLayout(hbox)
        layout.addWidget(QLabel("说话者:"))
        layout.addWidget(self.speaker_edit)
        layout.addWidget(QLabel("内容:"))
        layout.addWidget(self.text_edit)
        layout.addWidget(self.save_btn)
        self.setLayout(layout)
        self.nodes = []
        self.add_btn.clicked.connect(self.add_node)
        self.remove_btn.clicked.connect(self.remove_node)
        self.tree.currentItemChanged.connect(self.select_node)
        self.save_btn.clicked.connect(self.save_changes)

    def add_node(self):
        node = {"id": len(self.nodes)+1, "speaker": "", "text": "", "options": []}
        self.nodes.append(node)
        item = QTreeWidgetItem([f"节点 {node['id']}"])
        item.setData(0, 1, node['id'])
        self.tree.addTopLevelItem(item)

    def remove_node(self):
        item = self.tree.currentItem()
        if item:
            nid = item.data(0,1)
            self.nodes = [n for n in self.nodes if n['id'] != nid]
            self.tree.takeTopLevelItem(self.tree.indexOfTopLevelItem(item))

    def select_node(self, current, previous):
        if current:
            nid = current.data(0,1)
            for node in self.nodes:
                if node['id'] == nid:
                    self.speaker_edit.setText(node['speaker'])
                    self.text_edit.setText(node['text'])
                    break

    def save_changes(self):
        current = self.tree.currentItem()
        if current:
            nid = current.data(0,1)
            for node in self.nodes:
                if node['id'] == nid:
                    node['speaker'] = self.speaker_edit.text()
                    node['text'] = self.text_edit.toPlainText()
                    current.setText(0, f"节点 {nid} - {node['speaker']}")

    def get_data(self):
        return self.nodes

    def load_data(self, data):
        self.tree.clear()
        self.nodes = data
        for node in data:
            item = QTreeWidgetItem([f"节点 {node['id']} - {node.get('speaker','')}"])
            item.setData(0, 1, node['id'])
            self.tree.addTopLevelItem(item)
