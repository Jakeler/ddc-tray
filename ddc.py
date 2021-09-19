import ddc, sys

i2c_id = int(sys.argv[1])
dev = ddc.DDCCIDevice(i2c_id)
print(dev.brightness)