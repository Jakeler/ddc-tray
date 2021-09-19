import monitorcontrol, logging
logging.getLogger("monitorcontrol").setLevel(logging.DEBUG)

monitorcontrol.vcp.vcp_linux.LinuxVCP.GET_VCP_TIMEOUT = 0.1  # default 0.04
monitorcontrol.vcp.vcp_linux.LinuxVCP.CMD_RATE = 0.5 # default 0.05

mon1, mon2 = monitorcontrol.get_monitors()
with mon1:
    print('M1', mon1.get_luminance())
with mon2:
    print('M2', mon2.get_luminance())