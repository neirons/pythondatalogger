/* Includes ------------------------------------------------------------------*/
#include "NAND_FAT.h"


void NAND_FAT(void)
{
    FATFS fs;             /* Work area (file system object) for logical drives */
    FIL fsrc, fdst;       /* file objects */
    BYTE buffer[512];     /* file copy buffer */
    FRESULT res;          /* FatFs function common result code */
    UINT br, bw;          /* File R/W count */
   

    /* Register work area for logical drives */
    f_mount(0, &fs);
    
    /* Open source file on the drive 1 */
    res = f_open(&fsrc, "0:abc.txt", FA_OPEN_EXISTING | FA_READ);
    if (res) die(res);
    
    /* Create destination file on the drive 0 */
    res = f_open(&fdst, "0:copyofadb.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res) die(res);
  
    /* Copy source to destination */
    for (;;) {
        res = f_read(&fsrc, buffer, sizeof(buffer), &br);
        if (res || br == 0) break;   // error or eof
        res = f_write(&fdst, buffer, br, &bw);
        if (res || bw < br) break;   // error or disk full
    }
      
    /* Close open files */
    f_close(&fsrc);
    f_close(&fdst);

    /* Unregister work area prior to discard it */
    f_mount(0, NULL);
}

FRESULT die(FRESULT res)
{
// Add a breakpoint here for debbuging purpose
res = FR_OK;
return res;
}
