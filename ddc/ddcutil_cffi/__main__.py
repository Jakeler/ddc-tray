from . import DDC
print('MAIN')

ddc = DDC()
monitors = ddc.get_monitors()
for mon in monitors:
    print(mon)
    with ddc.open_monitor(mon) as m:
        ddc.read_vcp(m, DDC.VCP.BRIGHTNESS.value)
        # ddc.read_vcp(m, 0x1a) # blue gain
    print()
