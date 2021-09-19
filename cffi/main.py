from _ddc_cffi import ffi, lib

x = ffi.new('DDCA_Display_Info_List **')
ret = lib.ddca_get_display_info_list2(True, x)
print('code', ret)

lib.ddca_report_display_info_list(x[0], 2)

# de ref with [0] instead of *
monitor_count = x[0].ct
monitors = x[0].info

for mon_index in range(monitor_count):
    model = monitors[mon_index].model_name
    print(ffi.string(model))