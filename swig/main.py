import ddcutil
import _ddcutil as cddc

if __name__ == "__main__":
    out = ddcutil.DDCA_Display_Info_List()
    ret = ddcutil.ddca_get_display_info_list2(True, out)
    print(ret)
    print(out)
