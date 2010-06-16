/* Includes ------------------------------------------------------------------*/
#include "draw_bmp.h"



void draw_bmp(void)
{
	char filename[] = "Icon7.bmp";
  char filename2[] = "Ico_cale.bmp";
  char filename3[] = "Icon74.bmp";

  {
  LCD_WriteBMP_FAT(100,200,filename);
	LCD_WriteBMP_FAT(90,100,filename2);
  LCD_WriteBMP_FAT(239,319,filename3);
  LCD_Clear(Black);
  LCD_WriteBMP_FAT(200,200,"Ico_USBb.bmp");
  LCD_Clear(White);
  }
}

