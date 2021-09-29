from ddc_tray.ddc.ddcutil_cffi import DDC
print('MAIN')

ddc = DDC()
ddc.get_monitors()
for mon in ddc.monitors:
    print(mon)
    with ddc.open_monitor(mon) as m:
        # res = ddc.read_vcp(m, 0x1a) # blue gain
        ddc.write_vcp(m, DDC.VCP.BRIGHTNESS.value, 10)
        res = ddc.read_vcp(m, DDC.VCP.BRIGHTNESS.value)
        print(res)
    print()
