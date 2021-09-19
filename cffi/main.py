from _ddc_cffi import ffi, lib
import time

x = ffi.new('DDCA_Display_Info_List **')
ret = lib.ddca_get_display_info_list2(True, x)
print('code', ret)

lib.ddca_report_display_info_list(x[0], 0)

# de ref with [0] instead of *
monitor_count = x[0].ct
# no builtin iteration for further array deref, use generator/comprehension
monitors = (x[0].info[i] for i in range(monitor_count))

for mon in monitors:
    mfg = ffi.string(mon.mfg_id)
    model = ffi.string(mon.model_name)
    print(f'{mon.dispno}: [{mfg.decode()}] {model.decode()}')


    display_ref = mon.dref
    display_handle = ffi.new('DDCA_Display_Handle *')
    ret = lib.ddca_open_display2(display_ref, True, display_handle)

    vcp_val = ffi.new('DDCA_Any_Vcp_Value **')
    lib.ddca_get_any_vcp_value_using_explicit_type(display_handle[0], 0x10, 
        lib.DDCA_NON_TABLE_VCP_VALUE, vcp_val)
    print('Current', vcp_val[0].val.c_nc.sl)
    print('Max', vcp_val[0].val.c_nc.ml)

    ret = lib.ddca_close_display(display_handle[0])
    print(ret)

lib.ddca_free_display_info_list(x[0])