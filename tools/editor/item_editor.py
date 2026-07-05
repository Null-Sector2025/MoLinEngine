from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QListWidget, QPushButton, QTabWidget
from .property_editor import PropertyEditor

class ItemEditor(QWidget):
    def __init__(self):
        super().__init__()
        self.items = []
        self.skills = []
        self.tabs = QTabWidget()
        self.item_list = QListWidget()
        self.skill_list = QListWidget()
        self.prop = PropertyEditor()
        item_widget = QWidget()
        skill_widget = QWidget()
        # 物品 tab
        layout1 = QVBoxLayout()
        layout1.addWidget(self.item_list)
        hbox1 = QHBoxLayout()
        add_btn1 = QPushButton("添加物品")
        rem_btn1 = QPushButton("删除物品")
        hbox1.addWidget(add_btn1)
        hbox1.addWidget(rem_btn1)
        layout1.addLayout(hbox1)
        layout1.addWidget(self.prop)
        item_widget.setLayout(layout1)
        self.tabs.addTab(item_widget, "物品")
        add_btn1.clicked.connect(lambda: self.add_entry("item"))
        rem_btn1.clicked.connect(lambda: self.remove_entry("item"))
        self.item_list.currentRowChanged.connect(lambda idx: self.select_entry("item", idx))

    def add_entry(self, typ):
        if typ == "item":
            item = {"id": f"item_{len(self.items)+1}", "name": "新物品", "type": "consumable"}
            self.items.append(item)
            self.item_list.addItem(item['name'])
            self.item_list.setCurrentRow(len(self.items)-1)

    def remove_entry(self, typ):
        if typ == "item" and self.item_list.currentRow() >= 0:
            idx = self.item_list.currentRow()
            del self.items[idx]
            self.item_list.takeItem(idx)

    def select_entry(self, typ, idx):
        if typ == "item" and idx >= 0 and idx < len(self.items):
            schema = {"name": {"type":"str","label":"名称"}, "type": {"type":"combo","label":"类型","options":["consumable","weapon","armor"]}}
            self.prop.set_object(self.items[idx], schema)
