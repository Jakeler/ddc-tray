from PyQt5.QtGui import * 
from PyQt5.QtWidgets import * 
import os, signal
# Fix Ctrl-C, otherwise nothing happens
signal.signal(signal.SIGINT, signal.SIG_DFL)

WINDOW_TITLE = 'DDC Tray Settings'


def test():
    print('clicked some')
  
def toggle():
    # print(window.isHidden())
    if window.isHidden():
        window.show()
    else:
        window.hide()



app = QApplication([])
app.setQuitOnLastWindowClosed(False)

window = QWidget()
layout = QVBoxLayout()
btn_top = QPushButton('Top')
btn_bottom = QPushButton('Bottom')
layout.addWidget(btn_top)
layout.addWidget(btn_bottom)
window.setLayout(layout)
window.setWindowTitle(WINDOW_TITLE)
# window.show()

btn_top.clicked.connect(test)

# Adding an icon
base_path = os.path.dirname(__file__)
icon = QIcon(f"{base_path}/icons/custom_tray.png")
  
# Adding item on the menu bar
tray = QSystemTrayIcon(icon=icon)
tray.setIcon(icon)
tray.setVisible(True)
tray.activated.connect(toggle)

window.setWindowIcon(icon)
  
# Creating the options
context_menu = QMenu()
main_action = QAction("Open Settings")
main_action.triggered.connect(toggle)

auto_adj_toggle = QAction("Auto Adjust")
auto_adj_toggle.setCheckable(True)

sub_menu = QMenu('Monitor 1')
# sub_menu.addActions([option1, option2])

# To quit the app
quit_action = QAction("Quit")
quit_action.triggered.connect(app.quit)

context_menu.addAction(main_action)
context_menu.addAction(auto_adj_toggle)
context_menu.addSeparator()
context_menu.addMenu(sub_menu)
context_menu.addSeparator()
context_menu.addAction(quit_action)
  
# Adding options to the System Tray
tray.setContextMenu(context_menu)

def rem_acc():
    context_menu.removeAction(option1)
btn_bottom.clicked.connect(rem_acc)
  
app.exec_()
