/* File: ddcutil.i */
%module ddcutil

%include "typemaps.i"

%{
#define SWIG_FILE_WITH_INIT
#include "ddcutil_c_api.h"
#include "ddcutil_types.h"
%}


DDCA_Status
ddca_get_display_info_list2(
      bool                      include_invalid_displays,
      DDCA_Display_Info_List **OUTPUT);

//
// Display Information
//

/** Indicates how MCCS communication is performed */
typedef enum {
   DDCA_IO_I2C,     /**< Use DDC to communicate with a /dev/i2c-n device */
   DDCA_IO_ADL,     /**< Use ADL API */
   DDCA_IO_USB      /**< Use USB reports for a USB connected monitor */
} DDCA_IO_Mode;

/** ADL adapter number/display number pair, which identifies a display */
typedef struct {
   int iAdapterIndex;  /**< adapter number */
   int iDisplayIndex;  /**< display number */
} DDCA_Adlno;
// uses -1,-1 for unset

/** Describes a display's physical access mode and the location identifiers for that mode  */
typedef struct {
   DDCA_IO_Mode io_mode;        ///< physical access mode
   union {
      int        i2c_busno;     ///< I2C bus number
      DDCA_Adlno adlno;         ///< ADL iAdapterIndex/iDisplayIndex pair
      int        hiddev_devno;  ///* USB hiddev device  number
   } path;
} DDCA_IO_Path;

/** MCCS Version in binary form */
typedef struct {
   uint8_t    major;           /**< major version number */
   uint8_t    minor;           /*** minor version number */
} DDCA_MCCS_Version_Spec;

typedef void * DDCA_Display_Ref;

// Maximum length of strings extracted from EDID, plus 1 for trailing NULL
#define DDCA_EDID_MFG_ID_FIELD_SIZE 4
#define DDCA_EDID_MODEL_NAME_FIELD_SIZE 14
#define DDCA_EDID_SN_ASCII_FIELD_SIZE 14

typedef struct {
   char                   marker[4];        ///< always "DDIN"
   int                    dispno;           ///< ddcutil assigned display number
   DDCA_IO_Path           path;             ///< physical access path to display
   int                    usb_bus;          ///< USB bus number, if USB connection
   int                    usb_device;       ///< USB device number, if USB connection
   char                   mfg_id[    DDCA_EDID_MFG_ID_FIELD_SIZE    ]; ///< 3 character mfg id from EDID
   char                   model_name[DDCA_EDID_MODEL_NAME_FIELD_SIZE]; ///< model name from EDID, 13 char max
   char                   sn[        DDCA_EDID_SN_ASCII_FIELD_SIZE  ]; ///< "serial number" from EDID, 13 char max
   uint16_t               product_code;     ///< model product number
   uint8_t                edid_bytes[128];  ///< first 128 bytes of EDID
   DDCA_MCCS_Version_Spec vcp_version;      ///< VCP version as pair of numbers
   DDCA_Display_Ref       dref;             ///< opaque display reference
} DDCA_Display_Info;


/** Collection of #DDCA_Display_Info */
typedef struct {
   int                ct;       ///< number of records
   DDCA_Display_Info  info[];   ///< array whose size is determined by ct
} DDCA_Display_Info_List;