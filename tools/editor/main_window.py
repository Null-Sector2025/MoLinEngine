from PyQt5.QtWidgets import QMainWindow, QAction, QTabWidget, QFileDialog, QDockWidget, QMessageBox
from PyQt5.QtCore import Qt
from project_manager import ProjectManager, NewProjectDialog
from map_editor import MapEditor
from dialogue_editor import DialogueEditor
from quest_editor import QuestEditor
from item_editor import ItemEditor
from event_editor import EventEditor
from property_editor import PropertyEditor
import os

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.project = ProjectManager()
        self.setWindowTitle("墨麟编辑器 - MoLin Editor")
        self.setGeometry(100, 100, 1200, 800)
        self.create_actions()
        self.create_menus()
        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)
        # 创建编辑面板
        self.map_editor = MapEditor(self.project)
        self.dialogue_editor = DialogueEditor()
        self.quest_editor = QuestEditor()
        self.item_editor = ItemEditor()
        self.event_editor = EventEditor()
        self.tabs.addTab(self.map_editor, "地图")
        self.tabs.addTab(self.dialogue_editor, "对话")
        self.tabs.addTab(self.quest_editor, "任务")
        self.tabs.addTab(self.item_editor, "物品/技能")
        self.tabs.addTab(self.event_editor, "事件")

    def create_actions(self):
        self.new_act = QAction("新建项目", self)
        self.open_act = QAction("打开项目", self)
        self.save_act = QAction("保存项目", self)
        self.export_act = QAction("导出游戏数据", self)
        self.new_act.triggered.connect(self.new_project)
        self.open_act.triggered.connect(self.open_project)
        self.save_act.triggered.connect(self.save_project)
        self.export_act.triggered.connect(self.export_game)

    def create_menus(self):
        menubar = self.menuBar()
        file_menu = menubar.addMenu("文件")
        file_menu.addAction(self.new_act)
        file_menu.addAction(self.open_act)
        file_menu.addAction(self.save_act)
        file_menu.addAction(self.export_act)

    def new_project(self):
        dialog = NewProjectDialog(self)
        if dialog.exec_():
            path = dialog.created_path
            name = os.path.basename(path)
            self.project.new_project(path, name)
            self.setWindowTitle(f"墨麟编辑器 - {name}")

    def open_project(self):
        path = QFileDialog.getExistingDirectory(self, "选择项目目录")
        if path:
            self.project.open_project(path)
            self.setWindowTitle(f"墨麟编辑器 - {self.project.data.get('name','')}")

    def save_project(self):
        self.project.data["dialogues"] = self.dialogue_editor.get_data()
        self.project.save()
        QMessageBox.information(self, "保存", "项目已保存")

    def export_game(self):
        # 将项目数据导出为引擎可用的 JSON 文件
        export_dir = os.path.join(self.project.project_path, "export")
        os.makedirs(export_dir, exist_ok=True)
        import json
        # 导出对话
        with open(os.path.join(export_dir, "dialogues.json"), 'w') as f:
            json.dump(self.dialogue_editor.get_data(), f)
        # 导出其他...
        QMessageBox.information(self, "导出", f"游戏数据已导出到 {export_dir}")
