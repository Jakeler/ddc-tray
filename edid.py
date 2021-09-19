import pyedid, smbus, time, sys

i2c_id = int(sys.argv[1])
bus = smbus.SMBus(i2c_id)
bus.read_i2c_block_data(0x50, 0, 0) # reset pointer
time.sleep(0.5)
data = bytes([bus.read_byte(0x50) for n in range(256)])
print(data)

# returned Edid object, used the Default embedded registry
edid = pyedid.parse_edid(data)

print(edid)