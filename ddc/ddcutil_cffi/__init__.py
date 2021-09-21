import time
from ._ddc_cffi import ffi, lib
from ..interface import DDC_Interface as DDCi, DisplayCon
from contextlib import contextmanager

class DDC(DDCi):
    def get_monitors(self):
        x = ffi.new('DDCA_Display_Info_List **')
        ret = lib.ddca_get_display_info_list2(True, x)
        print('code', ret)
        # lib.ddca_report_display_info_list(x[0], 0)

        # de ref with [0] instead of *
        monitor_count = x[0].ct
        # no builtin iteration for further array deref, use generator/comprehension
        monitors = x[0].info
        return (self.Monitor(
            display_idx=monitors[i].dispno,
            display_ref=monitors[i].dref,
            model=ffi.string(monitors[i].model_name).decode(),
            manufacturer=ffi.string(monitors[i].mfg_id).decode(),
            vcp_ver=f'{monitors[i].vcp_version.major}.{monitors[i].vcp_version.minor}'
        ) for i in range(monitor_count))

    @contextmanager
    def open_monitor(self, mon: DDCi.Monitor):
        display_handle = ffi.new('DDCA_Display_Handle *')
        ret = lib.ddca_open_display2(mon.display_ref, True, display_handle)
        try:
            yield display_handle[0]
        finally:
            ret = lib.ddca_close_display(display_handle[0])

    def read_vcp(self, con: DisplayCon, code: int):
        vcp_val = ffi.new('DDCA_Any_Vcp_Value **')
        ret = lib.ddca_get_any_vcp_value_using_explicit_type(con, code, 
            lib.DDCA_NON_TABLE_VCP_VALUE, vcp_val)

        data = vcp_val[0].val.c_nc
        return self.VCP_result(
            value=data.sh << 8 | data.sl,
            max=data.mh << 8 | data.ml
        )

    def write_vcp(self):
        pass

# lib.ddca_free_display_info_list(x[0])