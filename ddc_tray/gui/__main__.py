from PyQt5.QtGui import * 
from PyQt5.QtWidgets import * 
import os, signal
# Fix Ctrl-C, otherwise nothing happens
signal.signal(signal.SIGINT, signal.SIG_DFL)

from ddc_tray.ddc.ddcutil_cffi import DDC, Monitor

ddc = DDC()
ddc.get_monitors()

WINDOW_TITLE = 'DDC Tray Settings'

def test():
    print('clicked some')
  
def toggle():
    # print(window.isHidden())
    if window.isHidden():
        window.show()
    else:
        window.hide()

def setMon(mon: Monitor, val: int):
    print('setting', mon, val)
    with ddc.open_monitor(mon) as m:
        ddc.write_vcp(m, DDC.VCP.BRIGHTNESS.value, val)

def generateMonitorActions(callback, step=10):
    actions = []
    for i in range(0, 100+1, step):
        act = QAction(f'{i} %')
        func = lambda _, val=i: callback(val)
        act.triggered.connect(func)
        actions.append(act)
    return actions

app = QApplication([])
app.setQuitOnLastWindowClosed(False)
# Adding an icon
base_path = os.path.dirname(__file__)
icon = QIcon(f"{base_path}/icons/custom_tray.png")

window = QWidget()
layout = QVBoxLayout()
btn_top = QPushButton('Top')
btn_bottom = QPushButton('Bottom')
layout.addWidget(btn_top)
layout.addWidget(btn_bottom)
window.setLayout(layout)
window.setWindowTitle(WINDOW_TITLE)
window.setWindowIcon(icon)
# window.show()
btn_top.clicked.connect(test)

# Adding item on the menu bar
tray = QSystemTrayIcon(icon=icon)
tray.setIcon(icon)
tray.setVisible(True)
tray.activated.connect(toggle)

# Creating the options
context_menu = QMenu()
main_action = QAction("Open Settings")
main_action.triggered.connect(toggle)

auto_adj_toggle = QAction("Auto Adjust")
auto_adj_toggle.setCheckable(True)


# To quit the app
quit_action = QAction("Quit")
quit_action.triggered.connect(app.quit)

context_menu.addAction(main_action)
context_menu.addAction(auto_adj_toggle)
context_menu.addSeparator()

actions = [None]*10
def genMenu(mon: Monitor) -> QMenu:
    menu = QMenu(str(mon))
    actions[mon.display_idx] = generateMonitorActions(lambda val, mon=mon: setMon(mon, val))
    menu.addActions(actions[mon.display_idx])
    print(menu)
    return menu

men = [None]*10
for mon in ddc.monitors:
    men[mon.display_idx] = genMenu(mon)
    context_menu.addMenu(men[mon.display_idx])

context_menu.addSeparator()
context_menu.addAction(quit_action)
  
# Adding options to the System Tray
tray.setContextMenu(context_menu)

def rem_acc():
    context_menu.removeAction(option1)
btn_bottom.clicked.connect(rem_acc)
  
app.exec_()
