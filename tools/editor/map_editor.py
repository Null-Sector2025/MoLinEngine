from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QScrollArea, QToolBar, QAction, QFileDialog, QLabel, QSpinBox
from PyQt5.QtCore import Qt, QRect, QPoint, pyqtSignal
from PyQt5.QtGui import QPainter, QPen, QColor, QPixmap, QImage
import os
import utils

class TileMapCanvas(QWidget):
    tileChanged = pyqtSignal(int, int, int)  # x, y, tile_id

    def __init__(self, map_width=20, map_height=15, tile_size=32):
        super().__init__()
        self.map_width = map_width
        self.map_height = map_height
        self.tile_size = tile_size
        self.tiles = [[0]*map_width for _ in range(map_height)]
        self.selected_tile = 0
        self.setFixedSize(map_width*tile_size, map_height*tile_size)
        self.setMouseTracking(True)

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(self.rect(), QColor(100,100,100))
        for y in range(self.map_height):
            for x in range(self.map_width):
                tid = self.tiles[y][x]
                # 简单绘制格子，颜色根据 tile_id 变化
                color = QColor(200,200,200) if tid==0 else QColor(50,150,200)
                painter.fillRect(x*self.tile_size, y*self.tile_size, self.tile_size, self.tile_size, color)
                painter.setPen(QPen(Qt.black, 1))
                painter.drawRect(x*self.tile_size, y*self.tile_size, self.tile_size, self.tile_size)
        painter.end()

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            x = event.pos().x() // self.tile_size
            y = event.pos().y() // self.tile_size
            if 0 <= x < self.map_width and 0 <= y < self.map_height:
                self.tiles[y][x] = self.selected_tile
                self.tileChanged.emit(x, y, self.selected_tile)
                self.update()

    def resize_map(self, width, height):
        # 简单实现，不保留旧数据
        self.map_width = width
        self.map_height = height
        self.tiles = [[0]*width for _ in range(height)]
        self.setFixedSize(width*self.tile_size, height*self.tile_size)
        self.update()

class MapEditor(QWidget):
    def __init__(self, project_manager):
        super().__init__()
        self.project = project_manager
        self.current_map = None
        self.canvas = TileMapCanvas()
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        toolbar = QToolBar()
        load_action = QAction("加载地图", self)
        save_action = QAction("保存地图", self)
        toolbar.addAction(load_action)
        toolbar.addAction(save_action)
        layout.addWidget(toolbar)
        self.scroll = QScrollArea()
        self.scroll.setWidget(self.canvas)
        layout.addWidget(self.scroll)
        self.setLayout(layout)
        load_action.triggered.connect(self.load_map)
        save_action.triggered.connect(self.save_map)

    def load_map(self):
        path, _ = QFileDialog.getOpenFileName(self, "打开地图", "", "JSON (*.json)")
        if path:
            data = utils.load_json(path)
            w = data.get("width", 20)
            h = data.get("height", 15)
            ts = data.get("tile_size", 32)
            self.canvas.map_width = w
            self.canvas.map_height = h
            self.canvas.tile_size = ts
            self.canvas.tiles = data.get("tiles", [[0]*w for _ in range(h)])
            self.canvas.setFixedSize(w*ts, h*ts)
            self.current_map = path
            self.canvas.update()

    def save_map(self):
        if not self.current_map:
            path, _ = QFileDialog.getSaveFileName(self, "保存地图", "", "JSON (*.json)")
            if not path: return
            self.current_map = path
        data = {
            "width": self.canvas.map_width,
            "height": self.canvas.map_height,
            "tile_size": self.canvas.tile_size,
            "tiles": self.canvas.tiles
        }
        utils.save_json(self.current_map, data)
