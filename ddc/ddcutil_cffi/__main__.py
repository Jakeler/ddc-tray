from . import DDC
print('MAIN')

ddc = DDC()
m = ddc.get_monitors()
ddc.read_vcp(m)