/* Includes ------------------------------------------------------------------*/
#include "NAND_FAT.h"
    FATFS fs;             /* Work area (file system object) for logical drives */


void NAND_FAT(void)
{
   

    /* Register work area for logical drives */
    f_mount(0, &fs);
    

    /* Unregister work area prior to discard it */
    //f_mount(0, NULL);
}

FRESULT die(FRESULT res)
{
// Add a breakpoint here for debbuging purpose
res = FR_OK;
return res;
}
