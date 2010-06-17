/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
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
#include "sdcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t Index = 0;
static __IO uint32_t AlarmStatus = 0;
static __IO uint32_t LedCounter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
uint32_t tmp = 318, index = 0;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  /* Disable LCD Window mode */
  LCD_WindowModeDisable(); 

  /* If HSE is not detected at program startup or HSE clock failed during program execution */
  if((Get_HSEStartUpStatus() == ERROR) || (RCC_GetITStatus(RCC_IT_CSS) != RESET))
  { 
    /* Clear the LCD */
    LCD_Clear(White);
    /* Set the LCD Back Color */
    LCD_SetBackColor(Blue);

    /* Set the LCD Text Color */
    LCD_SetTextColor(White);

    /* Display " No Clock Detected  " message */
    LCD_DisplayStringLine(Line0, "No HSE Clock        ");
    LCD_DisplayStringLine(Line1, "Detected. STANDBY   ");
    LCD_DisplayStringLine(Line2, "mode in few seconds.");
    
    LCD_DisplayStringLine(Line5, "If HSE Clock         ");
    LCD_DisplayStringLine(Line6, "recovers before the  ");
    LCD_DisplayStringLine(Line7, "time out, a System   ");
    LCD_DisplayStringLine(Line8, "Reset is generated.  ");
    LCD_ClearLine(Line9);
    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);

    GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    LCD_ClearLine(Line4);
    /* Set the Back Color */
    LCD_SetBackColor(White);
    /* Set the Text Color */
    LCD_SetTextColor(Red);
    LCD_DrawRect(71, 319, 25, 320);
    LCD_SetBackColor(Green); 
    LCD_SetTextColor(White);

    /* Wait till HSE is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {
      if(index == 0x3FFFF)
      {
        LCD_DisplayChar(Line3, tmp, 0x20);
        tmp -= 16;
        index = 0;
      }
      index++;
      /* Enters the system in STANDBY mode */
      if(tmp < 16)
      {
        LCD_SetBackColor(Blue);
        LCD_ClearLine(Line3);
        LCD_ClearLine(Line4);
        LCD_ClearLine(Line5);
        LCD_ClearLine(Line6);
        LCD_DisplayStringLine(Line7, " MCU in STANDBY Mode"); 
        LCD_DisplayStringLine(Line8, "To exit press Wakeup");
        /* Request to enter STANDBY mode */
        PWR_EnterSTANDBYMode();
      }
    }
  
    /* Generate a system reset */  
    NVIC_SystemReset();
  }
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
  /* Decrement the TimingDelay variable */
  Decrement_TimingDelay();
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  /* If counter is equal to 86339: one day was elapsed */
  if((RTC_GetCounter()/3600 == 23)&&(((RTC_GetCounter()%3600)/60) == 59)&&
     (((RTC_GetCounter()%3600)%60) == 59)) /* 23*3600 + 59*60 + 59 = 86339 */
  {
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset counter value */
    RTC_SetCounter(0x0);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Increment the date */
    Date_Update();
  }
  /* Clear the RTC Second Interrupt pending bit */  
  RTC_ClearITPendingBit(RTC_IT_SEC);
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    DownFunc();  
    /* Clear the EXTI Line 3 */
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
  if(Get_SmartCardStatus() == 0)
  { 
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
      /* Clear the EXTI Line 8 */  
      EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
      /* SEL function */
      Set_SELStatus();
      /* Clear the EXTI Line 7 */  
      EXTI_ClearITPendingBit(EXTI_Line7);
    }
  }
  else if(Get_SmartCardStatus() == 1)
  {
    if(EXTI_GetITStatus(SC_EXTI) != RESET)
    {
      /* Clear SC EXTIT Line Pending Bit */
      EXTI_ClearITPendingBit(SC_EXTI);

      /* Smartcard detected */
      Set_CardInserted();

      /* Power ON the card */
      SC_PowerCmd(ENABLE);

      /* Reset the card */
      SC_Reset(Bit_RESET);
    }
  }
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    /* Clear the EXTI Line 8 */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}


/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
  /* Clear the TIM1 Update pending bit */
  

  if(Index == 0)
  {
          GPIO_SetBits(GPIOF, GPIO_Pin_6);
          GPIO_ResetBits(GPIOF, GPIO_Pin_9);
          
          Index = 1;

  }
  else
  {
          GPIO_ResetBits(GPIOF, GPIO_Pin_6);
          GPIO_SetBits(GPIOF, GPIO_Pin_9);

          Index = 0;
  }

  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
  if ((SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET))
  {
    /* Send data on the SPI2 and Check the current commands */
    I2S_CODEC_DataTransfer();
  }
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
  /* If a Frame error is signaled by the card */
  if(USART_GetITStatus(USART3, USART_IT_FE) != RESET)
  {
    /* Clear the USART3 Frame error pending bit */
    USART_ClearITPendingBit(USART3, USART_IT_FE);
    USART_ReceiveData(USART3);

    /* Resend the byte that failed to be received (by the Smartcard) correctly */
    SC_ParityErrorHandler();
  }
  
  /* If the USART3 detects a parity error */
  if(USART_GetITStatus(USART3, USART_IT_PE) != RESET)
  {
    while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
    {
    }
    /* Clear the USART3 Parity error pending bit */
    USART_ClearITPendingBit(USART3, USART_IT_PE);
    USART_ReceiveData(USART3);
  }
  /* If a Overrun error is signaled by the card */
  if(USART_GetITStatus(USART3, USART_IT_ORE) != RESET)
  {
    /* Clear the USART3 Frame error pending bit */
    USART_ClearITPendingBit(USART3, USART_IT_ORE);
    USART_ReceiveData(USART3);
  }
  /* If a Noise error is signaled by the card */
  if(USART_GetITStatus(USART3, USART_IT_NE) != RESET)
  {
    /* Clear the USART3 Frame error pending bit */
    USART_ClearITPendingBit(USART3, USART_IT_NE);
    USART_ReceiveData(USART3);
  }
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line15) != RESET)
  {
    UpFunc();
    /* Clear the EXTI Line 15 */  
    EXTI_ClearITPendingBit(EXTI_Line15);
  }
 
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
  /* Clear the Alarm Pending Bit */
  RTC_ClearITPendingBit(RTC_IT_ALR);
  
  AlarmStatus = 1;
  Set_STOPModeStatus();

  /* Clear the EXTI Line 17/ */  
  EXTI_ClearITPendingBit(EXTI_Line17);
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
