/** @file ddcutil_c_api.h
 *
 *  Public C API for ddcutil
 */

// Copyright (C) 2014-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


/** @file ddcutil_types.h
 *  @brief File ddcutil_types.h contains type declarations for the C API.
 *
 *  API function declarations are specified in a separate file, ddcutil_c_api.h.
 *  The reason for this split is that the type declarations are used throughout the
 *  **ddcutil** implementation, whereas the function declarations are only referenced
 *  within the code that implements the API.
 */

// Copyright (C) 2014-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


//
// Status Code
//

/**
 * **ddcutil** Status Code
 *
 *  Most public **ddcutil** functions return a status code.
 *  These status codes have 3 sources:
 *  - Linux
 *  - ADL (AMD Display Library)
 *  - **ddcutil** itself
 *
 *  These multiple status code sources are consolidated by "modulating"
 *  the raw values into non-overlapping ranges.
 *  - Linux errno values are returned as negative numbers (e.g. -EIO)
 *  - ADL values are modulated by 2000 (i.e., 2000 subtracted from negative ADL status codes,
 *         or added to positive ADL status codes)
 *  - ddcutil errors are always in the -3000 range
 *
 *  In summary:
 *  - 0 always indicates a normal successful status
 *  - Positive values (possible with ADL) represent qualified success of some sort
 *  - Negative values indicate an error condition.
 */
typedef int DDCA_Status;


//
// Build Information
//

//! ddcutil version
//!
typedef struct {
   uint8_t    major;          ///< Major release number
   uint8_t    minor;          ///< Minor release number
   uint8_t    micro;          ///< Micro release number
} DDCA_Ddcutil_Version_Spec;

//! Build option flags, as returned by #ddca_build_options()
//! The enum values are defined as 1,2,4 etc so that they can be or'd.
typedef enum {
   /** @brief ddcutil was built with support for AMD Display Library connected monitors */
   DDCA_BUILT_WITH_ADL     = 0x01,
   /** @brief ddcutil was built with support for USB connected monitors */
   DDCA_BUILT_WITH_USB     = 0x02,
  /** @brief ddcutil was built with support for failure simulation */
   DDCA_BUILT_WITH_FAILSIM = 0x04
} DDCA_Build_Option_Flags;


//
// Error Reporting
//


//! Detailed error report
typedef struct ddca_error_detail {
   char                       marker[4];         ///< Always "EDTL"
   DDCA_Status                status_code;       ///< Error code
   char *                     detail;            ///< Optional explanation string
   uint16_t                   cause_ct;          ///< Number of sub-errors
   struct ddca_error_detail * causes[];          ///< Variable length array of contributing errors
} DDCA_Error_Detail;


//
// I2C Protocol Control
//

//! I2C timeout types
typedef enum{
   DDCA_TIMEOUT_STANDARD,      /**< Normal retry interval */
   DDCA_TIMEOUT_TABLE_RETRY    /**< Special timeout for Table reads and writes */
} DDCA_Timeout_Type;

//! I2C retry limit types
typedef enum{
   DDCA_WRITE_ONLY_TRIES,     /**< Maximum write-only operation tries */
   DDCA_WRITE_READ_TRIES,     /**< Maximum read-write operation tries */
   DDCA_MULTI_PART_TRIES      /**< Maximum multi-part operation tries */
} DDCA_Retry_Type;


//
// Message Control
//

//! Output Level
//!
//! Values assigned to constants allow them to be or'd in bit flags.
//!
//! Values are ascending in order of verbosity
typedef enum {
   DDCA_OL_TERSE  =0x04,         /**< Brief   output  */
   DDCA_OL_NORMAL =0x08,         /**< Normal  output */
   DDCA_OL_VERBOSE=0x10,         /**< Verbose output */
   DDCA_OL_VV=0x20               /**< Very verbose output */
} DDCA_Output_Level;


//
// Tracing
//

//! Trace Control
//!
//! Used as bitflags to specify multiple trace types
typedef enum {
 DDCA_TRC_BASE  = 0x0080,       /**< base functions          */
 DDCA_TRC_I2C   = 0x0040,       /**< I2C layer               */
 DDCA_TRC_ADL   = 0x0020,       /**< ADL layer               */
 DDCA_TRC_DDC   = 0x0010,       /**< DDC layer               */
 DDCA_TRC_USB   = 0x0008,       /**< USB connected display functions */
 DDCA_TRC_TOP   = 0x0004,       /**< ddcutil mainline        */
 DDCA_TRC_ENV   = 0x0002,       /**< environment command     */
 DDCA_TRC_API   = 0x0001,       /**< top level API functions */
 DDCA_TRC_UDF   = 0x0100,       /**< user-defined, aka dynamic, features */
 DDCA_TRC_VCP   = 0x0200,       /**< VCP layer, feature definitions */
 DDCA_TRC_DDCIO = 0x0400,       /**< DDC IO functions */
 DDCA_TRC_SLEEP = 0x0800,       /**< low level sleeps */
 DDCA_TRC_RETRY = 0x1000,       /**< successful retries, subset of DDCA_TRC_SLEEP */

 DDCA_TRC_NONE = 0x0000,        /**< all tracing disabled    */
 DDCA_TRC_ALL  = 0xffff         /**< all tracing enabled     */
} DDCA_Trace_Group;


typedef enum {
   DDCA_TRCOPT_TIMESTAMP = 0x01,
   DDCA_TRCOPT_THREAD_ID = 0x02,
} DDCA_Trace_Options;


//
// Performance statistics
//

//! Used as values to specify a single statistics type, and as
//! bitflags to select statistics types.
typedef enum {
   DDCA_STATS_NONE     = 0x00,    ///< no statistics
   DDCA_STATS_TRIES    = 0x01,    ///< retry statistics
   DDCA_STATS_ERRORS   = 0x02,    ///< error statistics
   DDCA_STATS_CALLS    = 0x04,    ///< system calls
   DDCA_STATS_ELAPSED  = 0x08,    ///< total elapsed time
   DDCA_STATS_ALL      = 0xFF     ///< indicates all statistics types
} DDCA_Stats_Type;


//
// Output capture
//

//! Capture option flags, used by #ddca_start_capture()
//!
//! The enum values are defined as 1,2,4 etc so that they can be or'd.
//!
//!  @since 0.9.0
typedef enum {
   DDCA_CAPTURE_NOOPTS     = 0,     ///< @brief no options specified
   DDCA_CAPTURE_STDERR     = 1      ///< @brief capture **stderr** as well as **stdout**
} DDCA_Capture_Option_Flags;


//
//  Display Specification
//

/** \defgroup api_display_spec API Display Specification */

/** Opaque display identifier
 *
 * A **DDCA_Display_Identifier** holds the criteria for selecting a monitor,
 * typically as specified by the user.
 *
 * It can take several forms:
 * - the display number assigned by **dccutil**
 * - an I2C bus number
 * - an ADL (adapter index, display index) pair
 * - a  USB (bus number, device number) pair or USB device number
 * - an EDID
 * - manufacturer, model, and serial number strings
 *
 * \ingroup api_display_spec
 * */
typedef void * DDCA_Display_Identifier;

/** Opaque display reference.
 *
 * A #DDCA_Display_Ref describes a monitor.  It contains 3 kinds of information:
 * - Assigned ddcutil display number
 * - The operating system path to the monitor, which is an I2C bus number, an
 *   ADL identifier, or a USB device number.
 * - Accumulated information about the monitor, such as the EDID or capabilities string.
 *
 * @remark
 * When libddcutil is started, it detects all connected monitors and creates
 * a persistent #DDCA_DisplayRef for each.
 * @remark
 * A #DDCA_Display_Ref can be obtained in 2 ways:
 * - From the DDCA_Display_List returned by #ddca_get_display_info_list2()
 * - Searching based on #DDCA_Display_Identifier using #ddca_get_display_ref()
 *
 * \ingroup api_display_spec
 */
typedef void * DDCA_Display_Ref;


/** Opaque display handle
 *
 * A **DDCA_Display_Handle** represents an "open" display on which actions can be
 * performed. This is required for communicating with a display. It is obtained by
 * calling #ddca_open_display2().
 *
 * For I2C and USB connected displays, an operating system open is performed by
 * # ddca_open_display2().  #DDCA_Display_Handle then contains the file handle
 * returned by the operating system.
 * For ADL displays, no actual operating system open is performed when creating
 * a DDCA_Display_Handle.  The adapter number.device number pair are simply copied
 * from the #DDCA_Display_Ref.
 *
 * \ingroup api_display_spec
 */
typedef void * DDCA_Display_Handle;

///@}


/** ADL adapter number/display number pair, which identifies a display */
typedef struct {
   int iAdapterIndex;  /**< adapter number */
   int iDisplayIndex;  /**< display number */
} DDCA_Adlno;
// uses -1,-1 for unset


//
// VCP Feature Information
//

/** MCCS Version in binary form */
typedef struct {
   uint8_t    major;           /**< major version number */
   uint8_t    minor;           /*** minor version number */
} DDCA_MCCS_Version_Spec;

extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_V10;        ///< MCCS version 1.0
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_V20;        ///< MCCS version 2.0
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_V21;        ///< MCCS version 2.1
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_V30;        ///< MCCS version 3.0
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_V22;        ///< MCCS version 2.2
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_ANY;        ///< used as query specifier
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_UNKNOWN;    ///< value for monitor has been queried unsuccessfully
extern const DDCA_MCCS_Version_Spec DDCA_VSPEC_UNQUERIED;  ///< indicates version not queried


/** MCCS VCP Feature Id */
typedef uint8_t DDCA_Vcp_Feature_Code;


/** Bitfield specifying a collection of VCP feature codes
 *
 *  @remark
 *  This struct might be more appropriately named DDCA_Feature_Set, but
 *  that results in confusing function names such as ddca_feature_set_set()
 */
typedef struct {
   uint8_t bytes[32];
} DDCA_Feature_List;

extern const DDCA_Feature_List DDCA_EMPTY_FEATURE_LIST;

/** Identifiers for publicly useful VCP feature subsets
 *
 * @remark
 * These subset identifiers represent a subset of the much
 * larger collection of subset ids used internally.
 */
typedef enum {
   DDCA_SUBSET_UNSET = 0,      ///< No subset selected
   DDCA_SUBSET_KNOWN,          ///< All features defined in a MCCS spec
   DDCA_SUBSET_COLOR,          ///< Color related features
   DDCA_SUBSET_PROFILE,        ///< Features saved and restored by loadvcp/setvcp
   DDCA_SUBSET_MFG,            ///< Feature codes reserved for manufacturer use (0x0e..0xff)
   DDCA_SUBSET_CAPABILITIES,   ///< Feature codes specified in capabilities string
   DDCA_SUBSET_SCAN,            ///< All feature codes other than known write-only or table
   DDCA_SUBSET_CUSTOM
} DDCA_Feature_Subset_Id;


//
// Display Information
//

/** Indicates how MCCS communication is performed */
typedef enum {
   DDCA_IO_I2C,     /**< Use DDC to communicate with a /dev/i2c-n device */
   DDCA_IO_ADL,     /**< Use ADL API */
   DDCA_IO_USB      /**< Use USB reports for a USB connected monitor */
} DDCA_IO_Mode;


/** Describes a display's physical access mode and the location identifiers for that mode  */
typedef struct {
   DDCA_IO_Mode io_mode;        ///< physical access mode
   union {
      int        i2c_busno;     ///< I2C bus number
      DDCA_Adlno adlno;         ///< ADL iAdapterIndex/iDisplayIndex pair
      int        hiddev_devno;  ///* USB hiddev device  number
   } path;
} DDCA_IO_Path;


// Maximum length of strings extracted from EDID, plus 1 for trailing NULL
#define DDCA_EDID_MFG_ID_FIELD_SIZE 4
#define DDCA_EDID_MODEL_NAME_FIELD_SIZE 14
#define DDCA_EDID_SN_ASCII_FIELD_SIZE 14



/** Describes one monitor detected by ddcutil. */
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


/** @name Version Feature Flags
 *
 * #DDCA_Version_Feature_Flags is a byte of flags describing attributes of a
 * VCP feature that can vary by MCCS version.
 *
 * @remark
 * Exactly 1 of #DDCA_RO, #DDCA_WO, #DDCA_RW is set.
 * @remark
 * Flags #DDCA_STD_CONT, #DDCA_COMPLEX_CONT, #DDCA_SIMPLE_NC, #DDCA_COMPLEX_NC,
 * #DDCA_WO_NC, #DDCA_NORMAL_TABLE, #DDCA_WO_TABLE refine  the C/NC/TABLE categorization
 * of the VESA MCCS specification.  Exactly 1 of these bits is set.
 */
///@{

/** Flags specifying VCP feature attributes, which can be VCP version dependent. */
typedef uint16_t DDCA_Version_Feature_Flags;

// Bits in DDCA_Version_Feature_Flags:

// Exactly 1 of DDCA_RO, DDCA_WO, DDCA_RW is set
#define DDCA_RO           0x0400               /**< Read only feature */
#define DDCA_WO           0x0200               /**< Write only feature */
#define DDCA_RW           0x0100               /**< Feature is both readable and writable */
#define DDCA_READABLE     0x0500          /**< Feature is either RW or RO */
#define DDCA_WRITABLE     0x0300          /**< Feature is either RW or WO */

// Further refine the C/NC/TABLE categorization of the MCCS spec
// Exactly 1 of the following 8 bits is set
#define DDCA_STD_CONT     0x0080       /**< Normal continuous feature */
#define DDCA_COMPLEX_CONT 0x0040       /**< Continuous feature with special interpretation */
#define DDCA_SIMPLE_NC    0x0020       /**< Non-continuous feature, having a defined list of values in byte SL */
#define DDCA_COMPLEX_NC   0x0010       /**< Non-continuous feature, having a complex interpretation using one or more of SL, SH, ML, MH */
#define DDCA_NC_CONT      0x0800       /**< NC feature combining reserved values with continuous range */
// For WO NC features.  There's no interpretation function or lookup table
// Used to mark that the feature is defined for a version
#define DDCA_WO_NC        0x0008       /**< Used internally for write-only non-continuous features */
#define DDCA_NORMAL_TABLE 0x0004       /**< Normal RW table type feature */
#define DDCA_WO_TABLE     0x0002       /**< Write only table feature */

#define DDCA_CONT         0x00c0            /**< Continuous feature, of any subtype */
#define DDCA_NC           0x0838            /**< Non-continuous feature of any subtype */
#define DDCA_NON_TABLE    0x08f8            /**< Non-table feature of any type */

#define DDCA_TABLE        0x0006        /**< Table type feature, of any subtype */
// #define DDCA_KNOWN     (DDCA_CONT | DDCA_NC | DDCA_TABLE)           // *** unused ***

// Additional bits:
#define DDCA_DEPRECATED   0x0001     /**< Feature is deprecated in the specified VCP version */

///@}

typedef uint16_t DDCA_Global_Feature_Flags;

// Bits in DDCA_Global_Feature_Flags:
#define DDCA_SYNTHETIC_VCP_FEATURE_TABLE_ENTRY  0x8000 /**< Used internally to indicate a temporary VCP_Feature_Table_Entry */
#define DDCA_USER_DEFINED 0x4000      /**< User provided feature definition */
// #define DDCA_SYNTHETIC_DDCA_FEATURE_METADATA 0x2000
#define DDCA_PERSISTENT_METADATA 0x1000  /**< Part of internal feature tables, do not free */
#define DDCA_SYNTHETIC     0x2000        /**< Generated feature definition  */

typedef uint16_t DDCA_Feature_Flags;    // union (DDCA_Version_Feature_Flags, DDCA_Global_Feature_Flags)


/** One entry in array listing defined simple NC values.
 *
 * An entry of {0x00,NULL} terminates the list.
 */
typedef
struct {
   uint8_t   value_code;
   char *    value_name;
} DDCA_Feature_Value_Entry;

// Makes reference to feature value table less implementation specific
// deprecated
// typedef DDCA_Feature_Value_Entry * DDCA_Feature_Value_Table;


/** Describes a VCP feature code, tailored for a specific monitor.
 *  Feature metadata can vary by VCP version and user defined features */
typedef
struct {
   char                                  marker[4];      /**< always "FMET" */
   DDCA_Vcp_Feature_Code                 feature_code;   /**< VCP feature code */
   DDCA_MCCS_Version_Spec                vcp_version;    /**< MCCS version    */
   DDCA_Feature_Flags                    feature_flags;  /**< feature type description */
   DDCA_Feature_Value_Entry *            sl_values;      /**< valid when DDCA_SIMPLE_NC set */
   DDCA_Feature_Value_Entry *            latest_sl_values;
   char *                                feature_name;   /**< feature name */
   char *                                feature_desc;   /**< feature description */
   // possibly add pointers to formatting functions
} DDCA_Feature_Metadata;


//
// Represent the Capabilities string returned by a monitor
//


/** Represents one feature code in the vcp() section of the capabilities string. */
typedef
struct {
   char                                 marker[4];     /**< Always DDCA_CAP_VCP_MARKER */
   DDCA_Vcp_Feature_Code                feature_code;  /**< VCP feature code */
   int                                  value_ct;      /**< number of values declared */
   uint8_t *                            values;        /**< array of declared values */
} DDCA_Cap_Vcp;


/** Represents a monitor capabilities string */
typedef
struct {
   char                                 marker[4];       /**< always DDCA_CAPABILITIES_MARKER */
   char *                               unparsed_string; /**< unparsed capabilities string */
   DDCA_MCCS_Version_Spec               version_spec;    /**< parsed mccs_ver() field */
   int                                  cmd_ct;          /**< number of command codes */
   uint8_t *                            cmd_codes;       /**< array of command codes */
   int                                  vcp_code_ct;     /**< number of features in vcp() field */
   DDCA_Cap_Vcp *                       vcp_codes;       /**< array of pointers to structs describing each vcp code */
   int                                  msg_ct;
   char **                              messages;
} DDCA_Capabilities;


//
// Get and set VCP feature values
//

/** Indicates the physical data type.  At the DDC level, continuous (C) and
 *  non-continuous (NC) features are treated identically.  They share the same
 *  DDC commands (Get VCP Feature and VCP Feature Reply) and data structure.
 *  Table (T) features use DDC commands Table Write and Table Read, which take
 *  different data structures.
 */
typedef enum {
   DDCA_NON_TABLE_VCP_VALUE = 1,   /**< Continuous (C) or Non-Continuous (NC) value */
   DDCA_TABLE_VCP_VALUE     = 2,       /**< Table (T) value */
} DDCA_Vcp_Value_Type;


typedef struct {
   uint8_t    mh;
   uint8_t    ml;
   uint8_t    sh;
   uint8_t    sl;
} DDCA_Non_Table_Vcp_Value;


/** Represents a single table VCP value.   Consists of a count, and a pointer to the bytes */
typedef struct {
   uint16_t bytect;        /**< Number of bytes in value */
   uint8_t*  bytes;        /**< Bytes of the value */
} DDCA_Table_Vcp_Value;


/** Stores a VCP feature value of any type */
typedef struct {
   DDCA_Vcp_Feature_Code  opcode;         /**< VCP feature code */
   DDCA_Vcp_Value_Type    value_type;      // probably a different type would be better
   union {
      struct {
         uint8_t *  bytes;          /**< pointer to bytes of table value */
         uint16_t   bytect;         /**< number of bytes in table value */
      }         t;                  /**< table value */
      struct {
         uint8_t    mh;
         uint8_t    ml;
         uint8_t    sh;
         uint8_t    sl;
      }    c_nc;                /**< continuous non-continuous, i.e. non-table, value */
   }       val;
} DDCA_Any_Vcp_Value;

// REMOVED MANUALLY
// #define VALREC_CUR_VAL(valrec) ( valrec->val.c_nc.sh << 8 | valrec->val.c_nc.sl )
// #define VALREC_MAX_VAL(valrec) ( valrec->val.c_nc.mh << 8 | valrec->val.c_nc.ml )



/** @file ddcutil_c_api.h
 *  @brief ddcutil public C API
 *
 *  Function names in the public C API begin with "ddca_"\n
 *  Status codes begin with "DDCRC_".\n
 *  Typedefs, other constants, etc. begin with "DDCA_".
 */


/* Note on "report" functions.
 *
 * Functions whose name begin with "ddca_report" or "ddca_dbgrpt" in the name,
 * e.g. ddca_report_display_ref(), ddca_report_display_info_list(), write formatted
 * reports to (normally) the terminal. Sometimes, these are intended to display
 * data structures for debugging.  Other times, they are used to format output
 * for the ddcutil command line program.
 *
 * The operation of these functions can be tweaked in two ways.
 * - The "depth" parameter is a logical indentation depth. This enables
 *   reports that invoke other reports to indent the subreports
 *   sensibly.  At the level of the ddcutil_c_api(), one unit of
 *   logical indentation depth translates to 3 spaces.
 * - The destination of reports is normally the STDOUT device.  This can
 *   be changed by calling set_fout().
 */


//
// Library build information
//

/**
 * Returns the ddcutil version as a struct of 3 8 bit integers.
 *
 * @return version numbers
 */
DDCA_Ddcutil_Version_Spec
ddca_ddcutil_version(void);

/**
 * Returns the ddcutil version as a string in the form "major.minor.micro".
 *
 * @return version string.
 */
const char *
ddca_ddcutil_version_string(void);


/** Queries the options with which the **ddcutil** library was built.
 *
 * @return  flags byte
 *
 * | Defined Bits: | |
 * |:-------| :--------------
 * |#DDCA_BUILT_WITH_USB  | built with USB support
 * |#DDCA_BUILT_WITH_FAILSIM | built with failure simulation
 *
 */
DDCA_Build_Option_Flags
ddca_build_options(void);


 //
 // Error Detail
 //

 /** Gets a copy of the detailed error information for the previous
  *  API call, if the call supports detailed error information (only a
  *  few do).
  *
  *  @return  copy of detailed error information (user must free)
  *
  *  @since 0.9.0
  */
 DDCA_Error_Detail *
 ddca_get_error_detail(void);

 /** Frees a detailed error information record
  *
  *  @param[in]  ddca_erec  error information to free
  *
  *  @since 0.9.0
  */
 void
 ddca_free_error_detail(
       DDCA_Error_Detail * ddca_erec);

 /** Issues a detailed report of a #DDCA_Error_Detail instance.
  *
  *  @param[in] ddca_erec  error information record
  *  @param[in] depth      logical indentation depth
  */
 void
 ddca_report_error_detail(
       DDCA_Error_Detail * ddca_erec,
       int                 depth);


//
// Status Codes
//

/** Returns the symbolic name for a ddcutil status code
 *
 * @param[in] status_code numeric status code
 * @return    symbolic name, e.g. EBUSY, DDCRC_INVALID_DATA
 * @retval    NULL if unrecognized code
 *
 * @remark
 * The returned value is a pointer into internal persistent
 * data structures and should not be freed by the caller.
 *  */
const char *
ddca_rc_name(
      DDCA_Status status_code);

/** Returns a description of a ddcutil status code
 *
 * @param[in] status_code numeric status code
 * @return    explanation of status code, e.g. "device or resource busy"
 * @retval    "unknown status code" if unrecognized code
 *
 * @remark
 * The returned value is a pointer into internal persistent
 * data structures and should not be free'd by the caller.
 */
const char *
ddca_rc_desc(
      DDCA_Status status_code);


//
// Global Settings
//

/***
I2C is an inherently unreliable protocol.  The application is responsible for
retry management.
The maximum number of retries can be tuned.
There are 3 retry contexts:
- An I2C write followed by a read.  Most DDC operations are of this form.
- An I2C write without a subsequent read.  DDC operations to set a VCP feature
  value are in this category.
- Some DDC operations, such as reading the capabilities string, reading table
  feature and writing table features require multiple
  write/read exchanges.  These multi-part exchanges have a separate retry count
  for the entire operation.
*/
///@{
/** Gets the upper limit on a max tries value that can be set.
 *
 * @return maximum max tries value allowed on set_max_tries()
 */
int
ddca_max_max_tries(void);

/** Gets the maximum number of I2C retries for the specified operation type.
 * @param[in]  retry_type   I2C operation type
 * @return maximum number of retries
 *
 * @remark
 * This setting is global, not thread-specific.
 */
int
ddca_get_max_tries(
    DDCA_Retry_Type retry_type);

/** Sets the maximum number of I2C retries for the specified operation type
 * @param[in] retry_type    I2C operation type
 * @param[in] max_tries     maximum count to set
 * @retval    DDCRC_ARG     max_tries < 1 or > #ddca_get_max_tries()
 *
 * @remark
 * This setting is global, not thread-specific.
 */
DDCA_Status
ddca_set_max_tries(
    DDCA_Retry_Type retry_type,
    int             max_tries);
///@}

/** Controls whether VCP values are read after being set.
 *
 * \param[in] onoff true/false
 * \return  prior value
 *
 * \remark This setting is thread-specific.
 */
bool
ddca_enable_verify(
      bool onoff);

/** Query whether VCP values are read after being set.
 * \retval true values are verified after being set
 * \retval false values are not verified
 *
 * \remark This setting is thread-specific.
 */
bool
ddca_is_verify_enabled(void);


/** Sets the sleep multiplier factor to be used for new threads.
 *
 *  \param[in]  multiplier
 *  \return     old multiplier
 *
 *  \remark
 *  This function is intended for use only during program initialization,
 *  typically from a value passed on the command line.
 *  Consequently there are no associated lock/unlock functions for the value.
 */
double
ddca_set_default_sleep_multiplier(double multiplier);

/** Gets the sleep multiplier factor used for new threads
 *
 * \return multiplier
 */
double
ddca_get_default_sleep_multiplier();


/** Sets the sleep multiplier factor for the current thread.
 *
 *  @param[in] multiplier
 *  @return    old multiplier
 */
double
ddca_set_sleep_multiplier(double multiplier);

/** Gets the sleep multiplier for the current thread
 *
 *  @return[in] sleep multiplier
 */
double
ddca_get_sleep_multiplier();


//
// Output Redirection
//

/** Redirects output on the current thread that normally would go to **stdout**  */
void
ddca_set_fout(
    FILE * fout);   /**< where to write normal messages, if NULL, suppress output */

/** Redirects output on the current thread that normally goes to **stdout** back to **stdout** */
void
ddca_set_fout_to_default(void);

/** Redirects output on the current thread that normally would go to **stderr**  */
void
ddca_set_ferr(
    FILE * ferr);   /**< where to write error messages, If NULL, suppress output */

/** Redirects output on the current thread that normally goes to **stderr** back to **stderr** */
void
ddca_set_ferr_to_default(void);


//
// Convenience functions for capturing output by redirecting
// to an in-memory buffer.
//

/** Begins capture of **stdout** and optionally **stderr** output on the
 *  current thread to a thread-specific in-memory buffer.
 *
 *  @note  If output is already being captured, this function has no effect.
 *  @since 0.9.0
 */
void
ddca_start_capture(
      DDCA_Capture_Option_Flags flags);

/** Ends capture of **stdout** output and returns the contents of the
 *  in-memory buffer.
 *
 *  Upon termination, normal thread output is directed to **stdout**.
 *  If error output was also being captured, error output is redirected
 *  to **stderr**.
 *
 *  @return captured output as a string.
 *
 *  @note
 *  If output is not currently being captured, returns a 0 length string.
 *  @note
 *  The returned string is valid until the next call to this function
 *  on the current thread.  It should not be freed by the caller.
 *
 *  @note  Writes messages to actual **stderr** in case of error.
 *  @since 0.9.0
 */
char *
ddca_end_capture(void);


//
// Message Control
//

/** Gets the current output level for the current thread
 *  @return      output level
 */
DDCA_Output_Level
ddca_get_output_level(void);

/** Sets the output level for the current thread
 *  @param[in]      new output level
 *  @return         prior output level
 */
DDCA_Output_Level
ddca_set_output_level(
      DDCA_Output_Level newval);

/** Gets the name of an output level
 *  @param[in]  val  output level id
 *  @return     output level name (do not free)
 */
char *
ddca_output_level_name(
      DDCA_Output_Level val);   /**< output level id */


/** Controls whether messages describing DDC protocol errors are output
 *  @param[in] onoff    if true, errors will be issued
 *  @return    prior value
 *
 *  This setting is global to all threads.
 */
bool
ddca_enable_report_ddc_errors(
      bool onoff);

/** Indicates whether messages describing DDC protocol errors are output.
 *
 *  This setting is global to all threads.
 */
bool
ddca_is_report_ddc_errors_enabled(void);


//
// Tracing
//

/** Turn on tracing for a specific function.
 *
 *  \param[in]  funcname   function name
 *
 *  \remark
 *  The function must include trace calls.
 */
void
ddca_add_traced_function(
      const char * funcname);

/** Turn on all tracing in a specific source file.
 *
 *  \param[in] filename  simple file name, with or without the ".c" extension,
 *                        e.g. vcp_feature_set.c, vcp_feature_set
 */
void
ddca_add_traced_file(
      const char * filename);

/* Specify one or more trace groups.
 *
 *  \param[in] trace_flags  bitfield indicating groups to trace
 */
void
ddca_set_trace_groups(
      DDCA_Trace_Group trace_flags);

/** Given a trace group name, returns its identifier.
 *  Case is ignored.
 *
 *  \param[in] name trace group name
 *  \return    trace group identifier
 *  \retval    TRC_NEVER unrecognized name
 */
DDCA_Trace_Group
ddca_trace_group_name_to_value(char * name);

/** Sets tracing options
 *
 *  \param[in] options  enum that can be used as bit flags
 */
void
ddca_set_trace_options(DDCA_Trace_Options  options);


//
// Performance Options
//

bool
ddca_enable_sleep_suppression(bool newval); ;

bool
ddca_is_sleep_suppression_enabled();


//
// Statistics and Diagnostics
//
// Statistics are global to all threads.
//

/** Resets all **ddcutil** statistics */
void
ddca_reset_stats(void);

/** Assigns a description to the the current thread.
 *
 *  \param[in] description
 */
void
ddca_set_thread_description(const char * description);

/** Appends text to the current thread description.
 *
 *  \param[in] description]
 */
void
ddca_append_thread_description(const char * description);
const char * ddca_get_thread_descripton();

/** Show execution statistics.
 *
 *  \param[in] stats  bitflags of statistics types to show
 *  \param[in] include_per_thread_data include per thread detail
 *  \param[in] depth  logical indentation depth
 */
void
ddca_show_stats(
      DDCA_Stats_Type stats,
      bool            include_per_thread_data,
      int             depth);

// TODO: Add functions to get stats

/** Enable display of internal exception reports (Error_Info).
 *
 *  @param[in] enable  true/false
 *  @return prior value
 */
bool
ddca_enable_error_info(
      bool enable);


//
// Display Descriptions
//

/** Controls whether USB devices are checked during display detection
 *
 *  Must be called before any API call that triggers display detection.
 *
 *  @param[in] onoff
 *  @retval    DDCRC_OK                success
 *  @retval    DDCRC_INVALID_OPERATION display detection has already occurred
 *  @retval    DDCRC_UNIMPLEMENTED     ddcutil not built with USB monitor support
 *
 *  @remark
 *  The default is to check USB devices.
 *
 *  This setting is global to all threads.
 */
DDCA_Status
ddca_enable_usb_display_detection(bool onoff);

/** Reports whether USB devices are checked as part of display detection
 *
 *  @retval true  USB devices are checked
 *  @retval valse UBB devices are not checked
 */
bool
ddca_ddca_is_usb_display_detection_enabled();

/** Gets a list of the detected displays.
 *
 *  @param[in]  include_invalid_displays if true, displays that do not support DDC are included
 *  @param[out] dlist_loc where to return pointer to #DDCA_Display_Info_List
 *  @retval     0  always succeeds
 */
DDCA_Status
ddca_get_display_info_list2(
      bool                      include_invalid_displays,
      DDCA_Display_Info_List**  dlist_loc);

/** Frees a list of detected displays.
 *
 *  This function understands which fields in the list
 *  point to permanently allocated data structures and should
 *  not be freed.
 *
 *  \param[in] dlist pointer to #DDCA_Display_Info_List
 */
void
ddca_free_display_info_list(
      DDCA_Display_Info_List * dlist);

/** Presents a report on a single display.
 *  The report is written to the current FOUT device for the current thread.
 *
 *  @param[in]  dinfo  pointer to a DDCA_Display_Info struct
 *  @param[in]  depth  logical indentation depth
 *
 *  @remark
 *  For a report intended for users, apply #ddca_report_display_by_dref()
 *  to **dinfo->dref**.
 */
void
ddca_report_display_info(
      DDCA_Display_Info * dinfo,
      int                 depth);

/** Reports on all displays in a list of displays.
 *  The report is written to the current FOUT device for the current thread.
 *
 *  @param[in]  dlist  pointer to a DDCA_Display_Info_List
 *  @param[in]  depth  logical indentation depth
 */
void
ddca_report_display_info_list(
      DDCA_Display_Info_List * dlist,
      int                      depth);

/** Reports on all active displays.
 *  This function hooks into the code used by command "ddcutil detect"
 *
 *  @param[in] include_invalid_displays if true, report displays that don't support DDC
 *  @param[in] depth  logical indentation depth
 *  @return    number of MCCS capable displays
 */
int
ddca_report_displays(
      bool include_invalid_displays,
      int  depth);


//
// Display Identifier
//

/** Creates a display identifier using the display number assigned by ddcutil
 * @param[in]  dispno  display number
 * @param[out] did_loc    where to return display identifier handle
 * @retval     0
 * \ingroup api_display_spec
 * */
DDCA_Status
ddca_create_dispno_display_identifier(
      int                      dispno,
      DDCA_Display_Identifier* did_loc);

/** Creates a display identifier using an I2C bus number
 * @param[in]  busno  I2C bus number
 * @param[out] did_loc   where to return display identifier handle
 * @retval     0
 *
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_create_busno_display_identifier(
      int                      busno,
      DDCA_Display_Identifier* did_loc);

/** Creates a display identifier using some combination of the manufacturer id,
 * model name string and serial number string.  At least 1 of the 3 must be specified.
 * @param[in]  mfg_id    3 letter manufacturer id
 * @param[in]  model     model name string
 * @param[in]  sn        serial number string
 * @param[out] did_loc   where to return display identifier handle
 * @retval     0         success
 * @retval     DDCRC_ARG all arguments NULL, or at least 1 too long
 *
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_create_mfg_model_sn_display_identifier(
      const char *             mfg_id,
      const char *             model,
      const char *             sn,
      DDCA_Display_Identifier* did_loc);

/** Creates a display identifier using a 128 byte EDID
 * @param[in]   edid       pointer to 128 byte EDID
 * @param[out]  did_loc    where to return display identifier handle
 * @retval      0          success
 * @retval      DDCRC_ARG  edid==NULL
 *
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_create_edid_display_identifier(
      const uint8_t*            edid,
      DDCA_Display_Identifier * did_loc);      // 128 byte edid

/** Creates a display identifier using a USB bus number and device number
 * @param[in]  bus    USB bus number
 * @param[in]  device USB device number
 * @param[out] did_loc   where to return display identifier handle
 * @retval 0 success
 *
 *  \ingroup api_display_spec
 */
DDCA_Status
ddca_create_usb_display_identifier(
      int                      bus,
      int                      device,
      DDCA_Display_Identifier* did_loc);

/** Creates a display identifier using a /dev/usb/hiddev device number
 * @param[in] hiddev_devno hiddev device number
 * @param[out] did_loc   where to return display identifier handle
 * @retval 0  success
 *
 *  \ingroup api_display_spec
 */
DDCA_Status
ddca_create_usb_hiddev_display_identifier(
      int                      hiddev_devno,
      DDCA_Display_Identifier* did_loc);


/** Release the memory of a display identifier
 * @param[in] did  display identifier, may be NULL
 * @retval 0          success
 * @retval DDCRC_ARG  invalid display identifier
 *
 * @remark
 * Does nothing and returns 0 if **did** is NULL.
 */
DDCA_Status
ddca_free_display_identifier(
      DDCA_Display_Identifier did);

/** Returns a string representation of a display identifier.
 *
 *  The string is valid until the display identifier is freed.
 *
 *  \param[in]  did    display identifier
 *  \return     string representation of display identifier, NULL if invalid
 *
 *  \ingroup api_display_spec
 */
const char *
ddca_did_repr(
      DDCA_Display_Identifier did);


//
// Display Reference
//

/**  \deprecated use #ddca_get_display_ref()
 *  Gets a display reference for a display identifier.
 *  Normally, this is a permanently allocated #DDCA_Display_Ref
 *  created by monitor detection and does not need to be freed.
 *  Use #ddca_free_display_ref() to safely free.
 * @param[in]  did      display identifier
 * @param[out] dref_loc where to return display reference
 * @retval     0                     success
 * @retval     DDCRC_ARG             did is not a valid display identifier handle
 * @retval     DDCRC_INVALID_DISPLAY display not found
 *
 * \ingroup api_display_spec
 */
// __attribute__ ((deprecated ("use ddca_get_display_ref()")))
DDCA_Status
ddca_create_display_ref(
      DDCA_Display_Identifier did,
      DDCA_Display_Ref*       dref_loc);


/** Gets a display reference for a display identifier.
 *  This is a permanently allocated #DDCA_Display_Ref
 *  created by monitor detection and does not need to be freed.
 * @param[in]  did      display identifier
 * @param[out] dref_loc where to return display reference
 * @retval     0                     success
 * @retval     DDCRC_ARG             did is not a valid display identifier handle
 * @retval     DDCRC_INVALID_DISPLAY display not found
 *
 * \since 0.9.5
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_get_display_ref(
      DDCA_Display_Identifier did,
      DDCA_Display_Ref*       dref_loc);

/**  \deprecated All display references are persistent
 *
 * Frees a display reference.
 *
 * Use this function to safely release a #DDCA_Display_Ref.
 * If the display reference was dynamically created, it is freed.
 * If the display reference was permanently allocated (normal case), does nothing.
 * @param[in] dref  display reference to free
 * @retval DDCRC_OK     success, or dref == NULL
 * @retval DDCRC_ARG    dref does not point to a valid display reference
 * @retval DDCRC_LOCKED dref is to a transient instance, and it is referenced
 *                      by an open display handle
 *
 * \ingroup api_display_spec
 */
// __attribute__ ((deprecated ("DDCA_Display_Refs are always persistent")))
DDCA_Status
ddca_free_display_ref(
      DDCA_Display_Ref dref);

/** Returns a string representation of a display reference
 *
 *  The returned value is valid until the next call to this function on
 *  the current thread.
 *
 *  @param[in]   dref display reference
 *  @return      string representation of display reference, NULL if invalid
 */
const char *
ddca_dref_repr(
      DDCA_Display_Ref dref);

/** Writes a report on the specified display reference to the current FOUT device
 * @param[in] dref   display reference
 * @param[in] depth  logical indentation depth
 *
 * \ingroup api_display_spec
 */
void
ddca_dbgrpt_display_ref(
      DDCA_Display_Ref dref,
      int              depth);


//
// Display Handle
//

/** Open a display
 * @param[in]  ddca_dref    display reference for display to open
 * @param[in]  wait         if true, wait if display locked by another thread
 * @param[out] ddca_dh_loc  where to return display handle
 * @return     status code
 *
 * Fails if display is already opened by another thread.
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_open_display2(
      DDCA_Display_Ref      ddca_dref,
      bool                  wait,
      DDCA_Display_Handle * ddca_dh_loc);

/** Close an open display
 * @param[in]  ddca_dh   display handle, if NULL do nothing
 * @retval     DDCRC_OK  close succeeded, or ddca_dh == NULL
 * @retval     DDCRC_ARG invalid handle
 * @return     -errno    from underlying OS close()
 *
 * \ingroup api_display_spec
 */
DDCA_Status
ddca_close_display(
      DDCA_Display_Handle   ddca_dh);

/** Returns a string representation of a display handle.
 *  The string is valid until until the handle is closed.
 *
 * @param[in] ddca_dh  display handle
 * @return string  representation of display handle, NULL if
 *                 argument is NULL or not a display handle
 *
 *  \ingroup api_display_spec
 */
const char *
ddca_dh_repr(
      DDCA_Display_Handle   ddca_dh);

// CHANGE NAME?
/** Returns the display reference for display handle.
 *
 *  @param[in] ddca_dh   display handle
 *  @return #DDCA_Display_Ref of the handle,
 *          NULL if invalid display handle
 *
 *  @since 0.9.0
 */
DDCA_Display_Ref
ddca_display_ref_from_handle(
      DDCA_Display_Handle   ddca_dh);


//
// Monitor Capabilities
//

/** Retrieves the capabilities string for a monitor.
 *
 *  @param[in]  ddca_dh     display handle
 *  @param[out] caps_loc    address at which to return pointer to capabilities string.
 *  @return     status code
 *
 *  It is the responsibility of the caller to free the returned string.
 */
DDCA_Status
ddca_get_capabilities_string(
      DDCA_Display_Handle     ddca_dh,
      char**                  caps_loc);

/** Parse the capabilities string.
 *
 *  @param[in] capabilities_string      unparsed capabilities string
 *  @param[out] parsed_capabilities_loc address at which to return pointer to newly allocated
 *                                      #DDCA_Capabilities struct
 *  @return     status code
 *
 *  It is the responsibility of the caller to free the returned struct
 *  using ddca_free_parsed_capabilities().
 *
 *  This function currently parses the VCP feature codes and MCCS version.
 *  It could be extended to parse additional information such as cmds if necessary.
 */
DDCA_Status
ddca_parse_capabilities_string(
      char *                   capabilities_string,
      DDCA_Capabilities **     parsed_capabilities_loc);

/** Frees a DDCA_Capabilities struct
 *
 *  @param[in] parsed_capabilities  pointer to struct to free,
 *                                  does nothing if NULL.
 */
void
ddca_free_parsed_capabilities(
      DDCA_Capabilities *      parsed_capabilities);

/** Reports the contents of a DDCA_Capabilities struct.
 *
 *  The report is written to the current FOUT location.
 *
 *  If the current output level is #DDCA_OL_VERBOSE, additional
 *  information is written, including command codes.
 *
 *  @param[in]  parsed_capabilities  pointer to #DDCA_Capabilities struct
 *  @param[in]  ddca_dref            display reference, may be NULL
 *  @param[in]  depth  logical       indentation depth
 *
 *  @remark
 *  If ddca_dref is not NULL, feature value names will reflect any loaded monitor definition files
 *  @since 0.9.3
 */
DDCA_Status
ddca_report_parsed_capabilities_by_dref(
      DDCA_Capabilities *      parsed_capabilities,
      DDCA_Display_Ref         ddca_dref,
      int                      depth);

/** Reports the contents of a DDCA_Capabilities struct.
 *
 *  The report is written to the current FOUT location.
 *
 *  If the current output level is #DDCA_OL_VERBOSE, additional
 *  information is written, including command codes.
 *
 *  @param[in]  parsed_capabilities  pointer to #DDCA_Capabilities struct
 *  @param[in]  ddca_dh              display handle, may be NULL
 *  @param[in]  depth                logical indentation depth
 *  @retval     0                    success
 *  @retval     DDCRC_ARG            invalid display handle
 *
 *  @remark
 *  If ddca_dh is not NULL, feature value names will reflect any loaded monitor definition files
 *  @since 0.9.3
 */
DDCA_Status
ddca_report_parsed_capabilities_by_dh(
      DDCA_Capabilities *      p_caps,
      DDCA_Display_Handle      ddca_dh,
      int                      depth);

/** Reports the contents of a DDCA_Capabilities struct.
 *
 *  The report is written to the current FOUT location.
 *
 *  If the current output level is #DDCA_OL_VERBOSE, additional
 *  information is written, including command codes.
 *
 *  @param[in]  parsed_capabilities  pointer to #DDCA_Capabilities struct
 *  @param[in]  ddca_dref            display reference
 *  @param[in]  depth  logical       indentation depth
 *
 *  @remark
 *  Any user supplied feature definitions for the monitor are ignored.
 */
void
ddca_report_parsed_capabilities(
      DDCA_Capabilities *      parsed_capabilities,
      int                      depth);

/** Returns the VCP feature codes defined in a
 *  parsed capabilities record as a #DDCA_Feature_LIst
 *
 *  @param[in] parsed_caps  parsed capabilities
 *  @return bitfield of feature ids
 *  @since 0.9.0
 */
DDCA_Feature_List
ddca_feature_list_from_capabilities(
      DDCA_Capabilities * parsed_caps);


//
//  MCCS Version Specification
//

/** Gets the MCCS version of a monitor.
 *
 *  @param[in]    ddca_dh   display handle
 *  @param[out]   p_vspec   where to return version spec
 *  @return       DDCRC_ARG invalid display handle
 *
 *  @remark Returns version 0.0 (#DDCA_VSPEC_UNKNOWN) if feature DF cannot be read
 */
DDCA_Status
ddca_get_mccs_version_by_dh(
      DDCA_Display_Handle     ddca_dh,
      DDCA_MCCS_Version_Spec* p_vspec);


//
// VCP Feature Metadata
//

/** Controls whether user defined features (aka dynamic features) are supported.
 *
 *  @param[in] onoff true/false
 *  @return    prior value
 *
 *  @since 0.9.3
 */
bool
ddca_enable_udf(bool onoff);

/** Query whether user defined features (aka dynamic features) are supported.
 *
 *  @retval true  UDF enabled
 *  @retval false UDF disabled
 *
 *  @since 0.9.3
 */
bool
ddca_is_udf_enabled(void);

/**
 * Loads any user supplied feature definition files for the specified
 * display.  Does nothing if they have already been loaded.
 *
 * @param[in] ddca_dref display reference
 *
 * @remark
 * User supplied feature definition files are not yet publicly supported.
 *
 * @since 0.9.3
 */
DDCA_Status
ddca_dfr_check_by_dref(DDCA_Display_Ref ddca_dref);

/**
 * Loads any user supplied feature definition files for the specified
 * display.  Does nothing if they have already been loaded.
 *
 * @param[in] ddca_dh display handle
 *
 * @remark
 * User supplied feature definition files are not yet publicly supported.
 * @since 0.9.3
 */
DDCA_Status
ddca_dfr_check_by_dh(DDCA_Display_Handle ddca_dh);

/**
 * Gets metadata for a VCP feature.
 *
 * @param[in]  vspec            VCP version
 * @param[in]  feature_code     VCP feature code
 * @param[in]  create_default_if_not_found
 * @param[out] meta_loc         return pointer to metadata here
 * @return     status code
 * @retval     DDCRC_ARG        invalid display handle
 * @retval     DDCRC_UNKNOWN_FEATURE unrecognized feature code and
 *                              !create_default_if_not_found
 *
 *  It is the responsibility of the caller to free the returned DDCA_Feature_Metadata instance.
 *
 * @remark
 * Note that VCP characteristics (C vs NC, RW vs RO, etc) can vary by MCCS version.
 * @remark
 * Only takes into account VCP version.  Useful for reporting display agnostic
 * feature information.  For display sensitive feature information, i.e. taking
 * into account the specific monitor model, use #ddca_get_feature_metdata_by_dref()
 * or #ddca_get_feature_metadata_by_dh().
 *
 * @since 0.9.3
 */
DDCA_Status
ddca_get_feature_metadata_by_vspec(
      DDCA_Vcp_Feature_Code       feature_code,
      DDCA_MCCS_Version_Spec      vspec,
      bool                        create_default_if_not_found,
      DDCA_Feature_Metadata **    meta_loc);

/**
 * Gets metadata for a VCP feature.
 *
 * Note that VCP characteristics (C vs NC, RW vs RO, etc) can vary by MCCS version.
 *
 * @param[in]  ddca_dref        display reference
 * @param[in]  feature_code     VCP feature code
 * @param[in]  create_default_if_not_found
 * @param[out] meta_loc         return pointer to metadata here
 * @return     status code
 * @retval     DDCRC_ARG        invalid display reference
 * @retval     DDCRC_UNKNOWN_FEATURE unrecognized feature code and
 *                              !create_default_if_not_found
 *
 * It is the responsibility of the caller to free the returned DDCA_Feature_Metadata instance.
 *
 * @remark
 * This function first checks if there is a user supplied feature definition
 * for the monitor.  If not, it looks up feature metadata based on the
 * VCP version of the monitor.
 * @remark
 * Note that feature characteristics (C vs NC, RW vs RO, etc) can vary by MCCS version.
 * @since 0.9.3
 */
DDCA_Status
ddca_get_feature_metadata_by_dref(
      DDCA_Vcp_Feature_Code       feature_code,
      DDCA_Display_Ref            ddca_dref,
      bool                        create_default_if_not_found,
      DDCA_Feature_Metadata **    meta_loc);

/**
 * Gets metadata for a VCP feature.
 *
 * @param[in]  ddca_dh          display handle
 * @param[in]  feature_code     VCP feature code
 * @param[in]  create_default_if_not_found
 * @param[out] meta_loc         return pointer to metadata here
 * @return     status code
 * @retval     DDCRC_ARG        invalid display handle
 * @retval     DDCRC_UNKNOWN_FEATURE unrecognized feature code and
 *                              !create_default_if_not_found
 *
 * It is the responsibility of the caller to free the returned DDCA_Feature_Metadata instance.
 *
 * @remark
 * This function first checks if there is a user supplied feature definition
 * for the monitor.  If not, it looks up feature metadata based on the
 * VCP version of the monitor.
 * @remark
 * Note that feature characteristics (C vs NC, RW vs RO, etc) can vary by MCCS version.
 * @since 0.9.3
 */
DDCA_Status
ddca_get_feature_metadata_by_dh(
      DDCA_Vcp_Feature_Code       feature_code,
      DDCA_Display_Handle         ddca_dh,
      bool                        create_default_if_not_found,
      DDCA_Feature_Metadata **    meta_loc);

/**
 *  Frees a #DDCA_Feature_Metadata instance.
 *
 *  @param[in] metadata pointer to instance
 *  @retval   0  normal
 *  @since 0.9.3
 *
 *  @remark
 *  It is not an error if the ***metadata*** pointer argument is NULL
 */
void
ddca_free_feature_metadata(DDCA_Feature_Metadata * metadata);

/** Gets the VCP feature name.  If different MCCS versions use different names
 *  for the feature, this function makes a best guess.
 *
 * @param[in]  feature_code feature code
 * @return     pointer to feature name (do not free), NULL if unknown feature code
 *
 * @remark
 * Since no specific display is indicated, this function ignores user defined
 * monitor feature information.
 */
const char *
ddca_get_feature_name(DDCA_Vcp_Feature_Code feature_code);


/** Convenience function that searches a Feature Value Table for a
 *  value and returns the corresponding name.
 *  @param[in]   feature_value_table  pointer to first entry of table
 *  @param[in]   feature_value        value to search for
 *  @param[out]  value_name_loc       where to return pointer to name
 *  @retval      DDCRC_OK  value found
 *  @retval      DDCRC_NOT_FOUND  value not found
 *
 * @remark
 * The value returned in **value_name_loc** is a pointer into the table
 * data structure.  Do not free.
 */
DDCA_Status
ddca_get_simple_nc_feature_value_name_by_table(
      DDCA_Feature_Value_Entry *  feature_value_table,
      uint8_t                     feature_value,
      char**                      value_name_loc);

/** Outputs a debugging report of the \DDCA_Feature_Metadata data structure.
 *
 *  @param[in] md    pointer to \DDCA_Feature_Metadata instance
 *  @param[in] depth logical indentation depth
 */
void
ddca_dbgrpt_feature_metadata(
      DDCA_Feature_Metadata * md,
      int                     depth);


//
//  Miscellaneous Monitor Specific Functions
//

/** Shows information about a display, specified by a #Display_Ref
 *
 * Output is written using report functions
 *
 * \param[in] dref       pointer to display reference
 * \param[in] depth      logical indentation depth
 * \retval DDCRC_ARG invalid display ref
 * \retval 0         success
 *
 * \remark
 * The detail level shown is controlled by the output level setting
 * for the current thread.
 *
 * @since 0.9.0
 */
DDCA_Status
ddca_report_display_by_dref(DDCA_Display_Ref dref, int depth);

//
// Feature Lists
//
// Specifies a collection of VCP features as a 256 bit array of flags.
//

/** Empty feature list
 *  @since 0.9.0
 */
extern const DDCA_Feature_List DDCA_EMPTY_FEATURE_LIST;

/** Returns feature list symbolic name (for debug messages)
 *
 *  @param[in] feature_set_id
 *  @return symbolic name (do not free)
 */
const char *
ddca_feature_list_id_name(
      DDCA_Feature_Subset_Id  feature_set_id);

/** Given a feature set id, returns a #DDCA_Feature_List specifying all the
 *  feature codes in the set.
 *
 *  @param[in]  feature_set_id
 *  @param[in]  dref                   display reference
 *  @param[in]  include_table_features if true, Table type features are included
 *  @param[out] points to feature list to be filled in
 *  @retval     DDCRC_ARG  invalid display reference
 *  @retval     DDCRC_OK   success
 *
 *  @since 0.9.0
 */
DDCA_Status
ddca_get_feature_list_by_dref(
      DDCA_Feature_Subset_Id  feature_set_id,
      DDCA_Display_Ref        dref,
      bool                    include_table_features,
      DDCA_Feature_List*      feature_list_loc);

/** Empties a #DDCA_Feature_List
 *
 *  @param[in]  vcplist pointer to feature list
 *
 *  @remark
 *  Alternatively, just set vcplist = DDCA_EMPTY_FEATURE_LIST
 *  @since 0.9.0
 */
void
ddca_feature_list_clear(
      DDCA_Feature_List* vcplist);


/** Adds a feature code to a #DDCA_Feature_List
 *
 *  @param[in]  vcplist   pointer to feature list
 *  @param[in]  vcp_code  VCP feature code
 *  #return     modified feature list
 *
 *  @remark
 *  The feature list is modified in place and also returned.
 *
 *  @since 0.9.0
 */
DDCA_Feature_List
ddca_feature_list_add(
      DDCA_Feature_List* vcplist,
      uint8_t vcp_code);

/** Tests if a #DDCA_Feature_List contains a VCP feature code
 *
 *  @param[in]  vcplist   feature list
 *  @param[in]  vcp_code  VCP feature code
 *  @return     true/false
 *
 *  @since 0.9.0
 */
bool
ddca_feature_list_contains(
      DDCA_Feature_List vcplist,
      uint8_t vcp_code);

/** Tests if 2 feature lists are equal.
 *
 *  @param[in] vcplist1   first feature list
 *  @param[in] vcplist2   second feature list
 *  @return true if they contain the same features, false if not
 *
 *  @remark
 *  The input feature lists are not modified.
 *  @since 0.9.9
 */
bool
ddca_feature_list_eq(
      DDCA_Feature_List vcplist1,
      DDCA_Feature_List vcplist2);

/** Creates a union of 2 feature lists.
 *
 *  @param[in] vcplist1   first feature list
 *  @param[in] vcplist2   second feature list
 *  @return feature list in which a feature is set if it is in either
 *          of the 2 input feature lists
 *
 *  @remark
 *  The input feature lists are not modified.
 *  @since 0.9.0
 */
DDCA_Feature_List
ddca_feature_list_or(
      DDCA_Feature_List vcplist1,
      DDCA_Feature_List vcplist2);

/** Creates the intersection of 2 feature lists.
 *
 *  @param[in] vcplist1   first feature list
 *  @param[in] vcplist2   second feature list
 *  @return feature list in which a feature is set if it is in both
 *          of the 2 input feature lists
 *
 *  @remark
 *  The input feature lists are not modified.
 *  @since 0.9.0
 */
DDCA_Feature_List
ddca_feature_list_and(
      DDCA_Feature_List vcplist1,
      DDCA_Feature_List vcplist2);

/** Returns a feature list consisting of all the features in the
 *  first list that are not in the second.
 *
 *  @param[in] vcplist1   first feature list
 *  @param[in] vcplist2   second feature list
 *  @return feature list in which a feature is set if it is in **vcplist1** but
 *          not **vcplist2**
 *
 *  @remark
 *  The input feature lists are not modified.
 *  @since 0.9.0
 */
DDCA_Feature_List
ddca_feature_list_and_not(
      DDCA_Feature_List vcplist1,
      DDCA_Feature_List vcplist2);

/** Returns the number of features in a feature list
 *
 *  @param[in] feature_list   feature list
 *  @return  number of features, 0 if feature_list == NULL
 *
 *  @since 0.9.0
 */
int
ddca_feature_list_count(
      DDCA_Feature_List feature_list);

/** Returns a string representation of a feature list as a
 *  sequence of 2 character hex values.
 *
 *  @param[in] feature_list   feature list
 *  @param[in] value_prefix   precede each value with this string, e.g. "0x"
 *                            if NULL, then no preceding string
 *  @param[in] sepstr         separator string between pair of values, e.g. ", "
 *                            if NULL, then no separator string
 *  @return    string representation; The value is valid until the next call
 *             to this function in the current thread.  Caller should not free.
 *
 *  @since 0.9.0
 */
const char *
ddca_feature_list_string(
      DDCA_Feature_List feature_list,
      const char * value_prefix,
      const char * sepstr);


//
// GET AND SET VCP VALUES
//

/*
 * The API for getting and setting VCP values is doubly specified,
 * with both functions specific to Non-Table and Table values,
 * and more generic functions that can handle values of any type.
 *
 * As a practical matter, Table type features have not been observed
 * on any monitors (as of 3/2018), and applications can probably
 * safely be implemented using only the Non-Table APIs.
 *
 * Note that the functions for #DDCA_Any_Vap_Value replace those
 * that previously existed for #DDCA_Single_Vcp_Value.
 */

//
// Free VCP Feature Value
//
// Note there is no function to free a #Non_Table_Vcp_Value, since
// this is a fixed size struct always allocated by the caller.
//

/** Frees a #DDCA_Table_Vcp_Value instance.
 *
 *  @param[in] table_value
 *
 *  @remark
 *  Was previously named **ddca_free_table_value_response().
 *  @since 0.9.0
 */
void
ddca_free_table_vcp_value(
      DDCA_Table_Vcp_Value * table_value);

/** Frees a #DDCA_Any_Vcp_Value instance.
 *
 *  @param[in] valrec  pointer to #DDCA_Any_Vcp_Value instance
 *  @since 0.9.0
 */
void
ddca_free_any_vcp_value(
      DDCA_Any_Vcp_Value * valrec);

/** Produces a debugging report of a #DDCA_Any_Vcp_Value instance.
 *  The report is written to the current FOUT device.
 *  @param[in]  valrec  instance to report
 *  @param[in]  depth   logical indentation depth
 *  @since 0.9.0
 */
// void
// dbgrpt_any_vcp_value(
//       DDCA_Any_Vcp_Value * valrec,
//       int                  depth);


//
// Get VCP Feature Value
//

/** Gets the value of a non-table VCP feature.
 *
 * @param[in]  ddca_dh       display handle
 * @param[in]  feature_code  VCP feature code
 * @param[out] valrec        pointer to response buffer provided by the caller,
 *                           which will be filled in
 * @return status code
 *
 * @remark
 * If the returned status code is other than **DDCRC_OK**, a detailed
 * error report can be obtained using #ddca_get_error_detail()
 * @remark
 * Renamed from **ddca_get_nontable_vcp_value()**
 * @since 0.9.0
 */
DDCA_Status
ddca_get_non_table_vcp_value(
       DDCA_Display_Handle        ddca_dh,
       DDCA_Vcp_Feature_Code      feature_code,
       DDCA_Non_Table_Vcp_Value*  valrec);

/** Gets the value of a table VCP feature.
 *
 * @param[in]  ddca_dh         display handle
 * @param[in]  feature_code    VCP feature code
 * @param[out] table_value_loc address at which to return the value
 * @return status code
 *
 * @remark
 * If the returned status code is other than **DDCRC_OK**, a detailed
 * error report can be obtained using #ddca_get_error_detail()
 * @note
 * Implemented, but untested
 */
DDCA_Status
ddca_get_table_vcp_value(
       DDCA_Display_Handle     ddca_dh,
       DDCA_Vcp_Feature_Code   feature_code,
       DDCA_Table_Vcp_Value ** table_value_loc);

/** Gets the value of a VCP feature of any type.
 *
 * @param[in]  ddca_dh       display handle
 * @param[in]  feature_code  VCP feature code
 * @param[in]  value_type    value type
 * @param[out] valrec_loc    address at which to return a pointer to a newly
 *                           allocated #DDCA_Any_Vcp_Value
 * @return status code
 *
 * @remark
 * If the returned status code is other than **DDCRC_OK**, a detailed
 * error report can be obtained using #ddca_get_error_detail()
 * @remark
 * Replaces **ddca_get_any_vcp_value()
 *
 * @since 0.9.0
 */
DDCA_Status
ddca_get_any_vcp_value_using_explicit_type(
       DDCA_Display_Handle         ddca_dh,
       DDCA_Vcp_Feature_Code       feature_code,
       DDCA_Vcp_Value_Type         value_type,
       DDCA_Any_Vcp_Value **       valrec_loc);

/** Gets the value of a VCP feature of any type.
 *  The type is determined by using ddcutil's internal
 *  feature description table.
 *
 *  Note that this function cannot be used for manufacturer-specific
 *  feature codes (i.e. those in the range xE0..xFF), since ddcutil
 *  does not know their type information.  Nor can it be used for
 *  unrecognized feature codes.
 *
 * @param[in]  ddca_dh       display handle
 * @param[in]  feature_code  VCP feature code
 * @param[out] valrec_loc    address at which to return a pointer to a newly
 *                           allocated #DDCA_Any_Vcp_Value
 * @return status code
 *
 * @remark
 * It an error to call this function for a manufacturer-specific feature or
 * an unrecognized feature.
 * @remark
 * If the returned status code is other than **DDCRC_OK**, a detailed
 * error report can be obtained using #ddca_get_error_detail()
 */
DDCA_Status
ddca_get_any_vcp_value_using_implicit_type(
       DDCA_Display_Handle         ddca_dh,
       DDCA_Vcp_Feature_Code       feature_code,
       DDCA_Any_Vcp_Value **       valrec_loc);


/** Returns a formatted representation of a table VCP value.
 *  It is the responsibility of the caller to free the returned string.
 *
 *  @param[in]  feature_code        VCP feature code
 *  @param[in]  dref                display reference
 *  @param[in]  table_value         table VCP value
 *  @param[out] formatted_value_loc address at which to return the formatted value.
 *  @return                         status code, 0 if success
 *  @since 0.9.0
 */
DDCA_Status
ddca_format_table_vcp_value_by_dref(
      DDCA_Vcp_Feature_Code   feature_code,
      DDCA_Display_Ref        ddca_dref,
      DDCA_Table_Vcp_Value *  table_value,
      char **                 formatted_value_loc);

/** Returns a formatted representation of a non-table VCP value.
 *  It is the responsibility of the caller to free the returned string.
 *
 *  @param[in]  feature_code        VCP feature code
 *  @param[in]  dref                display reference
 *  @param[in]  valrec              non-table VCP value
 *  @param[out] formatted_value_loc address at which to return the formatted value.
 *  @return                         status code, 0 if success
 *  @since 0.9.0
 */
DDCA_Status
ddca_format_non_table_vcp_value_by_dref(
      DDCA_Vcp_Feature_Code       feature_code,
      DDCA_Display_Ref            dref,
      DDCA_Non_Table_Vcp_Value *  valrec,
      char **                     formatted_value_loc);

/** Returns a formatted representation of a VCP value of any type
 *  It is the responsibility of the caller to free the returned string.
 *
 *  @param[in]  feature_code        VCP feature code
 *  @param[in]  dref                display reference
 *  @param[in]  valrec              non-table VCP value
 *  @param[out] formatted_value_loc address at which to return the formatted value.
 *  @return                         status code, 0 if success
 *  @since 0.9.0
 */
DDCA_Status
ddca_format_any_vcp_value_by_dref(
      DDCA_Vcp_Feature_Code   feature_code,
      DDCA_Display_Ref        dref,
      DDCA_Any_Vcp_Value *    valrec,
      char **                 formatted_value_loc);


//
// Set VCP value
//

/** Sets a non-table VCP value by specifying it's high and low bytes individually.
 *
 *  \param[in]   ddca_dh             display handle
 *  \param[in]   feature_code        feature code
 *  \param[in]   hi_byte             high byte of new value
 *  \param[in]   lo_byte             low byte of new value
 *  \return      status code
 */
DDCA_Status
ddca_set_non_table_vcp_value(
      DDCA_Display_Handle      ddca_dh,
      DDCA_Vcp_Feature_Code    feature_code,
      uint8_t                  hi_byte,
      uint8_t                  lo_byte
     );

/** Sets a Table VCP value.
 *
 *  \param[in]   ddca_dh             display handle
 *  \param[in]   feature_code        feature code
 *  \param[in]   new_value           value to set
 *  \return      status code
 *  \since 0.9.0
 */
DDCA_Status
ddca_set_table_vcp_value(
      DDCA_Display_Handle      ddca_dh,
      DDCA_Vcp_Feature_Code    feature_code,
      DDCA_Table_Vcp_Value *   new_value);

/** Sets a VCP value of any type.
 *
 *  \param[in]   ddca_dh        display handle
 *  \param[in]   feature_code   feature code
 *  \param[in]   new_value      value to set
 *  \return      status code
 *  \since 0.9.0
 */
DDCA_Status
ddca_set_any_vcp_value(
      DDCA_Display_Handle     ddca_dh,
      DDCA_Vcp_Feature_Code   feature_code,
      DDCA_Any_Vcp_Value *    new_value);


//
// Get or set multiple values
//
// These functions provide an API version of the **dumpvcp** and **loadvcp**
// commands.
//

/** Returns a string containing monitor identification and values
 *  for all detected features that should be saved when a monitor is
 *  calibrated and restored when the calibration is applied.
 *
 *  @param[in]  ddca_dh                   display handle
 *  @param[out] profile_values_string_loc address at which to return string
 *  @return     status code
 */
DDCA_Status
ddca_get_profile_related_values(
      DDCA_Display_Handle  ddca_dh,
      char**               profile_values_string_loc);

/** Sets multiple feature values for a specified monitor.
 *  The monitor identification and feature values are
 *  encoded in the string.
 *
 *  @param[in] ddca_dh display handle
 *  @param[in] profile_values_string string containing values
 *  @return     status code
 *
 *  @remark
 *  If **ddca_dh** is NULL, this function opens the first display
 *  that matches the display identifiers in the **profile_values_string**.
 *  If **ddca_dh** is non-NULL, then the identifiers in **profile_values_string**
 *  must be consistent with the open display.
 *  @remark
 *  The non-NULL case exists to handle the unusual situation where multiple
 *  displays have the same manufacturer, model, and serial number,
 *  perhaps because the EDID has been cloned.
 *  @remark
 *  If the returned status code is **DDCRC_BAD_DATA** (others?), a detailed
 *  error report can be obtained using #ddca_get_error_detail()
 */
DDCA_Status
ddca_set_profile_related_values(
      DDCA_Display_Handle  ddca_dh,
      char *               profile_values_string);


