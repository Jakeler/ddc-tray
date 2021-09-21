from . import DDC
print('MAIN')

ddc = DDC()
monitors = ddc.get_monitors()
for mon in monitors:
    print(repr(mon))
    with ddc.open_monitor(mon) as m:
        res = ddc.read_vcp(m, DDC.VCP.BRIGHTNESS.value)
        # res = ddc.read_vcp(m, 0x1a) # blue gain
        print(res)
    print()
