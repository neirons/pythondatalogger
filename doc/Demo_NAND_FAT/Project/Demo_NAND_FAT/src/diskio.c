/*-----------------------------------------------------------------------*/
/* MMC/SDSC/SDHC (in SPI mode) control module for STM32 Version 1.1.6    */
/* (C) Martin Thomas, 2010 - based on the AVR MMC module (C)ChaN, 2007   */
/*-----------------------------------------------------------------------*/

/* Copyright (c) 2010, Martin Thomas, ChaN
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */


#include "stm32f10x.h"
#include "ffconf.h"
#include "diskio.h"

// !!!!!!!!!!!!!!!!   COMMENT / UNCOMENT for using NAND / SD CARD !!!!!!!!!!!!!!!!!!
//#define SDCARD

#ifndef SDCARD
  #include "nand_if.h"
  #include "fsmc_nand.h"
#else
  #include "sdcard.h"
  SD_CardInfo SDCardInfo2;
#endif

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive number (0) */
)
{ 
#ifndef SDCARD
  NAND_Init();
#else
  NVIC_InitTypeDef NVIC_InitStructure;

  SD_Init();
  SD_GetCardInfo(&SDCardInfo2);
  SD_SelectDeselect((uint32_t) (SDCardInfo2.RCA << 16));
  SD_EnableWideBusOperation(SDIO_BusWide_4b);
  SD_SetDeviceMode(SD_DMA_MODE);
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive number (0) */
)
{
	return 0;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,			/* Physical drive number (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	uint16_t Transfer_Length;
	uint32_t Memory_Offset;

	Transfer_Length =  count * 512;
	Memory_Offset = sector * 512;
	
  #ifndef SDCARD
    NAND_Read(Memory_Offset, (uint32_t *)buff, Transfer_Length);
  #else
    SD_ReadBlock(Memory_Offset, (uint32_t *)buff, Transfer_Length);
  #endif

	return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE drv,			/* Physical drive number (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	uint16_t Transfer_Length;
	uint32_t Memory_Offset;

	Transfer_Length =  count * 512;
	Memory_Offset = sector * 512;

  #ifndef SDCARD
    NAND_Write(Memory_Offset, (uint32_t *)buff, Transfer_Length);
  #else
    SD_WriteBlock(Memory_Offset, (uint32_t *)buff, Transfer_Length);
  #endif

	return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/

DWORD get_fattime ()
{
	return	((2006UL-1980) << 25)	      // Year = 2006
			| (2UL << 21)	      // Month = Feb
			| (9UL << 16)	      // Day = 9
			| (22U << 11)	      // Hour = 22
			| (30U << 5)	      // Min = 30
			| (0U >> 1)	      // Sec = 0
			;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
 		
DRESULT disk_ioctl (
	BYTE drv,		// Physical drive number (0) 
	BYTE ctrl,		// Control code
	void *buff		// Buffer to send/receive control data
)
{		
	DRESULT res = RES_OK;
  #ifndef SDCARD
    uint32_t status = NAND_READY;
  #else
    uint32_t status = SD_NO_TRANSFER;
  #endif

		switch (ctrl) {
		case CTRL_SYNC :		/// Make sure that no pending write process
      #ifndef SDCARD
  			status = FSMC_NAND_GetStatus();
	  		if (status == NAND_READY)
      #else
			  status = SD_GetTransferState();
			  if (status == SD_NO_TRANSFER)
      #endif
				  {res = RES_OK;}
			  else{res = RES_ERROR;}
			break;

		case GET_SECTOR_COUNT :	  // Get number of sectors on the disk (DWORD)
			*(DWORD*)buff = 131072;	// 4*1024*32 = 131072
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE :	  // Get R/W sector size (WORD) 
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	    // Get erase block size in unit of sector (DWORD)
			*(DWORD*)buff = 32;
			res = RES_OK;
		  }
	  
	return res;
}
		  
