from PyQt5.QtGui import * 
from PyQt5.QtWidgets import * 

import os
import signal
# Fix Ctrl-C, otherwise nothing happens
signal.signal(signal.SIGINT, signal.SIG_DFL)


def test():
    print('clicked some')
  
app = QApplication([])
app.setQuitOnLastWindowClosed(False)

window = QWidget()
layout = QVBoxLayout()
btn_top = QPushButton('Top')
btn_bottom = QPushButton('Bottom')
layout.addWidget(btn_top)
layout.addWidget(btn_bottom)
window.setLayout(layout)
# window.show()

btn_top.clicked.connect(test)

def toggle():
    # print(window.isHidden())
    if window.isHidden():
        window.show()
    else:
        window.hide()


# Adding an icon
base_path = os.path.dirname(__file__)
icon = QIcon(f"{base_path}/icons/custom_tray.png")
  
# Adding item on the menu bar
tray = QSystemTrayIcon(icon=icon)
tray.setIcon(icon)
tray.setVisible(True)
tray.activated.connect(toggle)

# window.setWindowIcon(icon)
  
# Creating the options
menu = QMenu('Menu')
option1 = QAction("SOME")
option1.triggered.connect(test)

option2 = QAction("others")
option2.setCheckable(True)
menu.addAction(option1)
menu.addAction(option2)

submenu = QMenu('Monitor 1')
submenu.addActions([option1, option2])
menu.addSeparator()
menu.addMenu(submenu)
menu.addSeparator()

# To quit the app
quit = QAction("Quit")
quit.triggered.connect(app.quit)
menu.addAction(quit)
  
# Adding options to the System Tray
tray.setContextMenu(menu)

def rem_acc():
    menu.removeAction(option1)
btn_bottom.clicked.connect(rem_acc)
  
app.exec_()
