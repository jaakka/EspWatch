from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtGui import QPainter, QPen, QBrush, QFont
from PyQt5.QtCore import Qt

class MyWidget(QWidget):
    def paintEvent(self, event):
        self.resize(400, 400)
        self.setStyleSheet("background-color: black;")
        painter = QPainter(self)
        
        brush = QBrush(Qt.gray)  # Pallon täyttöväri
        painter.setBrush(brush)

        pen = QPen(Qt.gray, 5, Qt.SolidLine)
        painter.setPen(pen)
        painter.drawEllipse(25, 25, 350, 350)  # x, y, leveys, korkeus
        self.setWindowTitle('EspWatch simulator')

        pen = QPen(Qt.black, 5, Qt.SolidLine)
        painter.setPen(pen)

        font = QFont('Arial', 12)  # Arial fontti, 30 pisteen koko
        painter.setFont(font)

        painter.drawText(140, 190, 'System starting...')

app = QApplication([])
widget = MyWidget()
widget.show()
app.exec_()
