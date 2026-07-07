import os, json
from PyQt5.QtWidgets import QDialog, QVBoxLayout, QHBoxLayout, QLabel, QLineEdit, QPushButton, QFileDialog
from PyQt5.QtCore import QDir
import utils

class ProjectManager:
    def __init__(self):
        self.project_path = ""
        self.data = {
            "name": "未命名项目",
            "maps": {},
            "characters": [],
            "items": [],
            "skills": [],
            "dialogues": [],
            "quests": [],
            "events": []
        }

    def new_project(self, path, name):
        self.project_path = path
        self.data["name"] = name
        os.makedirs(os.path.join(path, "assets"), exist_ok=True)
        self.save()

    def open_project(self, path):
        self.project_path = path
        proj_file = os.path.join(path, "project.json")
        self.data = utils.load_json(proj_file)

    def save(self):
        if not self.project_path: return
        proj_file = os.path.join(self.project_path, "project.json")
        utils.save_json(proj_file, self.data)

    def get_map_path(self, map_name):
        return os.path.join(self.project_path, "maps", map_name + ".json")

class NewProjectDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("新建项目")
        self.path_edit = QLineEdit()
        self.name_edit = QLineEdit("我的RPG")
        btn_browse = QPushButton("浏览...")
        btn_create = QPushButton("创建")
        btn_cancel = QPushButton("取消")
        layout = QVBoxLayout()
        layout.addWidget(QLabel("项目名称:"))
        layout.addWidget(self.name_edit)
        layout.addWidget(QLabel("保存位置:"))
        hbox = QHBoxLayout()
        hbox.addWidget(self.path_edit)
        hbox.addWidget(btn_browse)
        layout.addLayout(hbox)
        hbox2 = QHBoxLayout()
        hbox2.addWidget(btn_create)
        hbox2.addWidget(btn_cancel)
        layout.addLayout(hbox2)
        self.setLayout(layout)
        btn_browse.clicked.connect(self.browse)
        btn_create.clicked.connect(self.create)
        btn_cancel.clicked.connect(self.reject)
        self.created_path = ""

    def browse(self):
        path = QFileDialog.getExistingDirectory(self, "选择保存目录")
        if path:
            self.path_edit.setText(path)

    def create(self):
        path = self.path_edit.text().strip()
        name = self.name_edit.text().strip()
        if not path or not name:
            return
        self.created_path = os.path.join(path, name)
        self.accept()
