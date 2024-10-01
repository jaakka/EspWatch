from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout
from PyQt5.QtGui import QPainter, QPen, QBrush, QFont
from PyQt5.QtCore import Qt

class MyWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Asetetaan pääikkuna ja sen koko
        self.setWindowTitle('EspWatch simulator')
        self.resize(400, 400)

        # Näytetään toinen ikkuna heti sovelluksen käynnistyessä
        self.openSecondWindow()

    def paintEvent(self, event):
        painter = QPainter(self)

        # Piirretään pallo
        brush = QBrush(Qt.gray)
        painter.setBrush(brush)
        pen = QPen(Qt.gray, 5, Qt.SolidLine)
        painter.setPen(pen)
        painter.drawEllipse(25, 25, 350, 350)

        # Piirretään teksti
        pen = QPen(Qt.black, 5, Qt.SolidLine)
        painter.setPen(pen)
        font = QFont('Arial', 12)
        painter.setFont(font)
        painter.drawText(140, 190, 'System starting...')

    # Metodi toisen ikkunan avaamiseksi
    def openSecondWindow(self):
        self.secondWindow = SecondWindow()
        self.secondWindow.show()

class SecondWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Asetetaan toisen ikkunan otsikko ja koko
        self.setWindowTitle('Toinen ikkuna')
        self.resize(300, 200)

        # Asetetaan taustaväri toiseen ikkunaan
        self.setStyleSheet("background-color: lightgray;")

app = QApplication([])
widget = MyWidget()
widget.show()
app.exec_()
