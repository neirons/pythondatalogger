/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : waveplayer.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Wave Player driver source file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define REPLAY  3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoTitle[20] = "STM32 I2S Codec Demo"; 

uint8_t CmdTitle0[20] = "  Control Buttons:  "; 

uint8_t CmdTitle1Playing[20] = "KEY>Pause  UP  >Vol+";

uint8_t CmdTitle2Playing[20] = "SEL>Stop   DOWN>Vol-";

uint8_t CmdTitle1Paused[20] =  "KEY>Play   UP  >Spkr";

uint8_t CmdTitle2Paused[20] =  "SEL>Stop   DOWN>Head";

uint8_t CmdTitle1Stopped[20] = "    UP > Speaker    "; 

uint8_t CmdTitle2Stopped[20] = "  DOWN > Headphone  "; 

uint8_t StatusTitleStopped[20] = "      Stopped       ";

uint8_t StatusTitlePlaying[20] = "      Playing       ";

uint8_t StatusTitlePaused[20] = "       Paused       ";

uint8_t i2cerr[20] =  "ERROR:I2C com. ->RST";
uint8_t memerr[20] =  "ERROR: Memory  ->RST";
uint8_t fileerr[20] = "ERROR: No Wave File ";

static uint8_t previoustmp = 50;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LCD_Update
* Description    : Controls the wave player application LCD display messages.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Update(uint32_t Status)
{
  uint8_t tmp = 0;
  uint32_t counter = 0;

  /* Enable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  switch (Status)
  {
   case PROGRESS:
         tmp = (uint8_t) ((uint32_t)((GetVar_AudioDataIndex()) * 100) / GetVar_AudioDataLength());
         if (tmp == 0)
         { 
           LCD_SetTextColor(Magenta);
           LCD_ClearLine(Line8);
           LCD_DrawRect(Line8, 310, 16, 300);
         }
         else
         {
           LCD_SetTextColor(Magenta);
           LCD_DrawLine(Line8, 310 - (tmp * 3), 16, Vertical);
         }         
         break;
   case FRWD:
         tmp = (uint8_t) ((uint32_t)((GetVar_AudioDataIndex()) * 100) / GetVar_AudioDataLength());

         LCD_SetTextColor(Magenta);
         LCD_ClearLine(Line8);
         LCD_DrawRect(Line8, 310, 16, 300);
         LCD_SetTextColor(Magenta);

         for (counter = 0; counter <= tmp; counter++)
         {
           LCD_DrawLine(Line8, 310 - (counter * 3), 16, Vertical);
         }          
         break;
   case STOP:
         /* Display the stopped status menu */ 
         LCD_SetTextColor(White); 
         LCD_DisplayStringLine(Line3, CmdTitle1Stopped);
         LCD_DisplayStringLine(Line4, CmdTitle2Stopped);
         LCD_SetTextColor(Red);
         LCD_DisplayStringLine(Line6, StatusTitleStopped);
         LCD_ClearLine(Line9);
         LCD_SetTextColor(Black);
         LCD_DisplayChar(Line9, 250, 'v'); 
         LCD_DisplayChar(Line9, 235, 'o'); 
         LCD_DisplayChar(Line9, 220, 'l'); 
         LCD_DisplayChar(Line9, 200, '-'); 
         LCD_DisplayChar(Line9, 85, '+'); 
         LCD_DrawRect(Line9 + 8, 185, 10, 100); 
         break; 
   case PAUSE: 
         /* Display the paused status menu */ 
         LCD_SetTextColor(White);
         LCD_DisplayStringLine(Line3, CmdTitle1Paused);
         LCD_DisplayStringLine(Line4, CmdTitle2Paused);
         LCD_SetTextColor(Red);
         LCD_DisplayStringLine(Line6, StatusTitlePaused);
         break;
   case PLAY:
         /* Display the Titles */   
         LCD_SetTextColor(Black);
         LCD_DisplayStringLine(Line0, DemoTitle);
         LCD_DisplayStringLine(Line2, CmdTitle0); 

         /* Display the Playing status menu */ 
         LCD_SetTextColor(White);
         LCD_DisplayStringLine(Line3, CmdTitle1Playing);
         LCD_DisplayStringLine(Line4, CmdTitle2Playing);
         LCD_SetTextColor(Red);
         LCD_DisplayStringLine(Line6, StatusTitlePlaying);
         LCD_ClearLine(Line9);
         LCD_SetTextColor(Black);
         LCD_DisplayChar(Line9, 250, 'v'); 
         LCD_DisplayChar(Line9, 235, 'o'); 
         LCD_DisplayChar(Line9, 220, 'l'); 
         LCD_DisplayChar(Line9, 200, '-'); 
         LCD_DisplayChar(Line9, 85, '+'); 
         LCD_DrawRect(Line9 + 8, 185, 10, 100); 
         break;
   case ALL: 
         I2S_CODEC_LCDConfig();
         /* Display the stopped status menu */ 
         LCD_SetTextColor(White); 
         LCD_DisplayStringLine(Line3, CmdTitle1Stopped);
         LCD_DisplayStringLine(Line4, CmdTitle2Stopped);
         LCD_SetTextColor(Red);
         LCD_DisplayStringLine(Line6, StatusTitleStopped);
         LCD_ClearLine(Line9);
         LCD_SetTextColor(Black);
         LCD_DisplayChar(Line9, 250, 'v'); 
         LCD_DisplayChar(Line9, 235, 'o'); 
         LCD_DisplayChar(Line9, 220, 'l'); 
         LCD_DisplayChar(Line9, 200, '-'); 
         LCD_DisplayChar(Line9, 85, '+'); 
         LCD_DrawRect(Line9 + 8, 185, 10, 100); 
         break;
  }
  /* Update the volume bar in all cases except when progress bar is to be apdated */
  if (Status != PROGRESS)
  {
    /* Compute the current volume percentage */
    tmp = (uint8_t) ((uint16_t)((0xFF - GetVar_CurrentVolume()) * 100) / 0xFF) ;
 
    /* Clear the previuos volume bar */
    LCD_SetTextColor(Blue);
    LCD_DrawLine(Line9 + 10, 185 - previoustmp , 8, Vertical);
    LCD_DrawLine(Line9 + 10, 185 - previoustmp + 1 , 8, Vertical);    
 
    /* Draw the new volume bar */
    LCD_SetTextColor(Red);
    LCD_DrawLine(Line9 + 10, 185 - tmp , 8, Vertical);
    LCD_DrawLine(Line9 + 10, 185 - tmp + 1 , 8, Vertical);
 
    /* save the current position */
    previoustmp = tmp;
  }
  /* Disable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
}

/*******************************************************************************
* Function Name  : LCD_DisplayError
* Description    : Displays error message on the LCD screen and prompt user to 
*                  reset the application.
* Input          : err: Error code.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayError(uint32_t err)
{
  I2S_CODEC_LCDConfig();

  /* Enable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  LCD_SetTextColor(Red); 

  /* Clear the LCD */
  LCD_Clear(White);


  /* The memory initialazation failed */
  if (err == 1)
  {
    LCD_DisplayStringLine(Line7, memerr); 
  }

  /* The audio file initialization failed (wrong audio format or wrong file) */
  if (err == 2)
  {
    LCD_DisplayStringLine(Line7, fileerr); 
  }

  /* I2C communication failure occured */
  if (err == 3)
  {
    LCD_DisplayStringLine(Line7, i2cerr); 
  }

  LCD_DisplayStringLine(Line8, "Push JoyStick to    ");
  LCD_DisplayStringLine(Line9, "exit.               ");

  /* Disable the FSMC that share a pin w/ I2C1 (LBAR) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);

  while(ReadKey() == NOKEY)
  {
  }
}

/*******************************************************************************
* Function Name  : I2S_CODEC_LCDConfig
* Description    : Initialize the LCD device and display the welcome screen.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2S_CODEC_LCDConfig(void)
{
  /* Set the text and the background color */
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(Black);
  LCD_Clear(White);

  /* Display the Titles */  
  LCD_DisplayStringLine(Line0, DemoTitle);
  LCD_DisplayStringLine(Line2, CmdTitle0); 
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
