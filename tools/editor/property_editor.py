from PyQt5.QtWidgets import QWidget, QFormLayout, QLineEdit, QSpinBox, QComboBox, QVBoxLayout, QPushButton
from PyQt5.QtCore import pyqtSignal

class PropertyEditor(QWidget):
    valueChanged = pyqtSignal(dict)

    def __init__(self):
        super().__init__()
        self.properties = {}
        self.widgets = {}
        self.layout = QFormLayout()
        self.setLayout(self.layout)

    def set_object(self, obj, schema):
        # 清空
        for w in self.widgets.values():
            self.layout.removeWidget(w)
            w.deleteLater()
        self.widgets.clear()
        self.properties = obj
        # 根据 schema 创建控件
        for key, spec in schema.items():
            if spec["type"] == "int":
                w = QSpinBox()
                w.setValue(obj.get(key, 0))
            elif spec["type"] == "str":
                w = QLineEdit()
                w.setText(obj.get(key, ""))
            elif spec["type"] == "combo":
                w = QComboBox()
                w.addItems(spec.get("options", []))
                w.setCurrentText(obj.get(key, ""))
            else:
                continue
            self.layout.addRow(spec.get("label", key), w)
            self.widgets[key] = w
            w.valueChanged.connect(self.emit_change) if hasattr(w, 'valueChanged') else w.textChanged.connect(self.emit_change)

    def emit_change(self):
        for key, w in self.widgets.items():
            if isinstance(w, QSpinBox):
                self.properties[key] = w.value()
            elif isinstance(w, QLineEdit):
                self.properties[key] = w.text()
            elif isinstance(w, QComboBox):
                self.properties[key] = w.currentText()
        self.valueChanged.emit(self.properties)
