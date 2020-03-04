#ifndef MEMORY_USB_HOST_MSC_H_
#define MEMORY_USB_HOST_MSC_H_
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "usblib/usblib.h"
#include "usblib/usbmsc.h"
#include "usblib/host/usbhost.h"
#include "usblib/host/usbhmsc.h"
#include "third_party/fatfs/src/ff.h"
#include "third_party/fatfs/src/diskio.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
//#include "utils/uartstdio.h"
//#include "utils/cmdline.h"
#include "flash.h"



//
// Defines the size of the buffers that hold the path, or temporary
// data from the USB disk.  There are two buffers allocated of this size.
// The buffer size must be large enough to hold the int32_test expected
// full path name, including the file name, and a trailing null character.
//
//*****************************************************************************
#define PATH_BUF_SIZE   80
#define NUM_SSI_DATA            6
//*****************************************************************************
//
// Defines the number of times to call to check if the attached device is
// ready.
//
//*****************************************************************************
#define USBMSC_DRIVE_RETRY      4
#define CMD_BUF_SIZE            64

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.
// Initially it is root ("/").
//
//*****************************************************************************
static char g_pcCwdBuf[PATH_BUF_SIZE] = "/";
static char g_pcTmpBuf[PATH_BUF_SIZE] = "POLYGO~1.GCO";

//*****************************************************************************
//
// The buffer that holds the command line.
//
//*****************************************************************************
static char g_pcCmdBuf[CMD_BUF_SIZE];


//*****************************************************************************
//
// A structure that holds a mapping between an FRESULT numerical code,
// and a string representation.  FRESULT codes are returned from the FatFs
// FAT file system driver.
//
//*****************************************************************************

typedef struct
{
    FRESULT fresult;
    char *pcResultStr;
}
tFresultString;

//*****************************************************************************
//
// A macro to make it easy to add result codes to the table.
//
//*****************************************************************************
#define FRESULT_ENTRY(f)        { (f), (#f) }


//*****************************************************************************
//
// A macro that holds the number of result codes.
//
//*****************************************************************************
#define NUM_FRESULT_CODES (sizeof(g_sFresultStrings) / sizeof(tFresultString))

//*****************************************************************************
//
// Error reasons returned by ChangeToDirectory().
//
//*****************************************************************************
#define NAME_TOO_LONG_ERROR 1
#define OPENDIR_ERROR       2

//*****************************************************************************
//
// Storage for the filenames.
//
//*****************************************************************************
#define NUM_LIST_STRINGS 48
const char *g_ppcDirListStrings[NUM_LIST_STRINGS];

//*****************************************************************************
//
// Storage for the names of the files in the current directory.  Filenames
// are stored in format "(D) filename.ext" for directories or "(F) filename.ext"
// for files.
//
//*****************************************************************************
#define MAX_FILENAME_STRING_LEN (4 + 8 + 1 + 3 + 1)
char g_pcFilenames[NUM_LIST_STRINGS][MAX_FILENAME_STRING_LEN];

//*****************************************************************************
//
// Storage for the strings which appear in the status box at the bottom of the
// display.
//
//****************************************************************************
#define NUM_STATUS_STRINGS 6
#define MAX_STATUS_STRING_LEN (36 + 1)
char g_pcStatus[NUM_STATUS_STRINGS][MAX_STATUS_STRING_LEN];


//*****************************************************************************
//
// Flag indicating that some USB device is connected.
//
//*****************************************************************************
#define FLAGS_DEVICE_PRESENT    0x00000001

//*****************************************************************************
//
// The size of the host controller's memory pool in bytes.
//
//*****************************************************************************
#define HCD_MEMORY_SIZE         128

//*****************************************************************************
//
// The control table used by the uDMA controller.  This table must be aligned
// to a 1024 byte boundary.  In this application uDMA is only used for USB,
// so only the first 6 channels are needed.
//
//*****************************************************************************
#if defined(ewarm)
#pragma data_alignment=1024
tDMAControlTable g_psDMAControlTable[6];
#elif defined(ccs)
#pragma DATA_ALIGN(g_psDMAControlTable, 1024)
tDMAControlTable g_psDMAControlTable[6];
#else
tDMAControlTable g_psDMAControlTable[6] __attribute__ ((aligned(1024)));
#endif

//*****************************************************************************
//
// Define a pair of buffers that are used for holding path information.
// The buffer size must be large enough to hold the longest expected
// full path name, including the file name, and a trailing null character.
// The initial path is set to root "/".
//
//*****************************************************************************
#define PATH_BUF_SIZE   80

//*****************************************************************************
//
// Define the maximum number of files that can appear at any directory level.
// This is used for allocating space for holding the file information.
// Define the maximum depth of subdirectories, also used to allocating space
// for directory structures.
// Define the maximum number of characters allowed to be stored for a file
// name.
//
//*****************************************************************************
#define MAX_FILES_PER_MENU 64
#define MAX_SUBDIR_DEPTH 32


void hardware_init(void);
static bool FileInit(void);
static const char *StringFromFresult(FRESULT fresult);
static void MSCCallback(tUSBHMSCInstance *ps32Instance, uint32_t ui32Event, void *pvData);
int read_file(int argc, char *argv[]);
void usbInit(void);
void usbConnect(void);
int printFileStructure (void);
void uartInit(void);

#endif /* MEMORY_USB_HOST_MSC_H_ */
