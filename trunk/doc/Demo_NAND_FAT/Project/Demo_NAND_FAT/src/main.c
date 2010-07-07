/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Main program body
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay = 0;
static __IO ErrorStatus HSEStartUpStatus = SUCCESS;

static __IO uint8_t USB_Plugin_State = 0;
static uint16_t record_count = 0;

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

//#define ADC3_DR_Address    ((uint32_t)0x40013C4C)
#define ADC3_DR_Address    (ADC1_DR_Address)

__IO uint16_t ADC1ConvertedValue = 0, ADC2ConvertedValue = 0;


FIL  g_file_datalogger;
void RTC_Init(void);
#define BKP_POWER_ON BKP_DR8

#define FLAG_POWER_ON 0x5A5A
#define FLAG_POWER_OFF 0x4B4B

#define FLAG_FLASH_READY 0x3C3C
#define FLAG_FLASH_NOT_READY 0x0000


#define DATA_PAGES 32

#define DATA_LOGGER_ADDRESS_START  ((uint32_t)0x08008000)


#define FLASH_READY_ADDRESS  ((uint32_t)DATA_LOGGER_ADDRESS_START + 1024 * DATA_PAGES + 10 )
#define REOCRD_COUNT_ADDRESS  ((uint32_t)DATA_LOGGER_ADDRESS_START + 1024 * (DATA_PAGES + 2 + 10))



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
  Board_main();
}
void Board_main(void)
{   
    uint8_t i;
    uint16_t flash_flag = 0;
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
    
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
        
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1); 
        
        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_2);
        
        /* Enable PLL */ 
        RCC_PLLCmd(ENABLE);
        
        /* Wait till PLL is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }
        
        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    
    /* Enable GPIOA, GPIOB, and AFIO clocks */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
    


    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  
    /* Enable write access to Backup domain */
    PWR_BackupAccessCmd(ENABLE);
    
    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

  
    /*------------------- Resources Initialization -----------------------------*/
    /* GPIO Configuration */
    GPIO_Config();
    
    /* Interrupt Configuration */
    InterruptConfig();
    
    /* Configure the systick */    
    SysTick_Configuration();
    
    /*------------------- Drivers Initialization -------------------------------*/
    
    /* Initialize the Low Power application */
    LowPower_Init();
    
    
    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);
    
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    
    
    RTC_Init();  
    
    
    /* If HSE is not detected at program startup */
    if(HSEStartUpStatus == ERROR)
    {
        /* Generate NMI exception */
        SCB->ICSR |= SCB_ICSR_NMIPENDSET;
    }  
    

    
    
    WakupPin_Init();
    CheckPowerOnReason();

    /*init the flag*/  
    flash_flag = *(uint16_t *)FLASH_READY_ADDRESS;
    if( flash_flag != FLAG_FLASH_READY)
    {
        FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
        //Erase the 32 page 32K
        for(i = 0;i< DATA_PAGES;i++)
        {
            FLASH_ErasePage(DATA_LOGGER_ADDRESS_START + i * 1024);
        }
        FLASH_ErasePage(FLASH_READY_ADDRESS);
        FLASH_ProgramHalfWord(FLASH_READY_ADDRESS , FLAG_FLASH_READY);

        FLASH_ErasePage(REOCRD_COUNT_ADDRESS);        
        FLASH_ProgramHalfWord(REOCRD_COUNT_ADDRESS , 0x0000);
        
        FLASH_Lock();

    }
    
    
    USB_Plugin_State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    
    if(USB_Plugin_State == 0)
    {
        
      
        /* Enable DMA1 ,DMA2 clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);
        
        /* Enable ADC1 ADC2,and GPIOC clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_ADC2| RCC_APB2Periph_GPIOC, ENABLE);
      
        Board_ADC_Init();        
        record_count =   *(uint16_t *)REOCRD_COUNT_ADDRESS;
        
        if(record_count >= 15428)
        {
            //Write the full flag
            //Do nothing....          
          while(1)
          {
            Led_One_By_One(4);
          }
        }
        else
        {
            FLASH_Unlock();
            FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
            FLASH_ProgramHalfWord(DATA_LOGGER_ADDRESS_START + record_count * 2, GetTemperature());
            //Erase first
            FLASH_ErasePage(REOCRD_COUNT_ADDRESS);
            //Update the count
            record_count = record_count + 1;
            
            FLASH_ProgramHalfWord(REOCRD_COUNT_ADDRESS , record_count);
            FLASH_Lock();
            
            
            //Each 10 point 
            if(record_count % 10 == 0)
            {
                Enable_SDcard();
                NAND_FAT();  
                CreateDataLoggerFile();                
                Disable_SDcard();        
                          
            }            
        }
        
      /* Disable DMA1 ,DMA2 clock */
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, DISABLE);
      
      /* Disable ADC1 ADC2,and GPIOC clock */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_ADC2| RCC_APB2Periph_GPIOC, DISABLE);
      
      GPIO_SetBits(GPIOA, GPIO_Pin_1);
      Delay(25);

    }
    else
    {
      
        Enable_SDcard();
        /*
        if there is usb connect, copy the data to sdcard. and start the mass storage
        */
        Mass_Storage_Start ();     

        while( bDeviceState != CONFIGURED)
        {
        }
#if 0        
        while( bDeviceState == CONFIGURED)
        {
        }
#else
        USB_Plugin_State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
        while(    USB_Plugin_State == 1)
        {
                  USB_Plugin_State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
        }
#endif        
        //Power off
//        Led_Power_Off();                
        
        PowerOff();    
        Disable_SDcard();        
        BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_OFF); 
        PWR_EnterSTANDBYMode();    

        /* Generate a system reset */  
        //NVIC_SystemReset();    
    }
    
    
    /* Set the RTC Alarm after 60s */
    RTC_SetAlarm(RTC_GetCounter()+ 3);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
    PWR_EnterSTANDBYMode();    
    
    
}

/*******************************************************************************
* Function Name  : InterruptConfig
* Description    : Configures the used IRQ Channels and sets their priority.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InterruptConfig(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
    
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* Enable the USB_LP_CAN_RX0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the USB_HP_CAN_TX Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
 
    
    
      /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SysTick_Configuration(void)
{
    /* Setup SysTick Timer for 10 msec interrupts  */
    if (SysTick_Config(SystemFrequency / 100))
    { 
        /* Capture error */ 
        while (1);
    }
    
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);
}




/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configures the different GPIO ports pins.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Configure PB14 for usb detect*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /* PA.01, PA.02 as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    TimingDelay = nCount;
    
    /* Enable the SysTick Counter */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
    
    while(TimingDelay != 0);
    
    /* Disable the SysTick Counter */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
    
    /* Clear the SysTick Counter */
    SysTick->VAL = (uint32_t)0x0;
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}


void WakupPin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    /* PA.01, input push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
}
/*******************************************************************************
* Function Name  : Get_HSEStartUpStatus
* Description    : Returns the HSE StartUp Status.
* Input          : None
* Output         : None
* Return         : HSE StartUp Status.
*******************************************************************************/
ErrorStatus Get_HSEStartUpStatus(void)
{
    return (HSEStartUpStatus);
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    LCD_Clear(White);
    LCD_DisplayStringLine(Line0, "   Assert Function  ");
    
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

void CreateDataLoggerFile()
{
    FRESULT res;          /* FatFs function common result code */
    UINT  bw;          /* File R/W count */
    
    uint8_t i = 0;    
    uint16_t value = 0;  
    
    record_count =   *(uint16_t *)REOCRD_COUNT_ADDRESS;
    
    FIL*  pfile = &g_file_datalogger;
    
    /* Create destination file on the drive 0 */
    res = f_open(pfile, "0:datalogger.bin", FA_OPEN_ALWAYS|FA_WRITE);
    if (res) die(res);
    
    res = f_lseek(pfile, pfile->fsize,0);

    
    for(i =record_count - 10;i<record_count;i++)
    {
        value = *(uint16_t*)(DATA_LOGGER_ADDRESS_START + i * 2);
        res = f_write(pfile, &value, sizeof(value), &bw);
    }
    f_close(pfile);
    
    
    
/*Write the battery file*/
    res = f_open(pfile, "0:battery.bin", FA_CREATE_ALWAYS | FA_WRITE);
    value = GetBatteryInfo();
    res = f_write(pfile, &value, sizeof(value), &bw);    
    if (res) die(res);
    f_close(pfile);

    f_mount(0, NULL);

}

void RTC_Init(void)
{
    /* Check if the StandBy flag is set */
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {/* System resumed from STANDBY mode */
        
        
        /* Clear StandBy flag */
        PWR_ClearFlag(PWR_FLAG_SB);
        
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
        /* No need to configure the RTC as the RTC configuration(clock source, enable,
        prescaler,...) is kept after wake-up from STANDBY */
    }
    else
    {/* StandBy flag is not set */
        
        /* RTC clock source configuration ----------------------------------------*/
        /* Reset Backup Domain */
        BKP_DeInit();
        
        /* Enable LSE OSC */
        RCC_LSEConfig(RCC_LSE_ON);
        /* Wait till LSE is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
        }
        
        /* Select the RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        
        /* Enable the RTC Clock */
        RCC_RTCCLKCmd(ENABLE);
        
        /* RTC configuration -----------------------------------------------------*/
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
        
        /* Set the RTC time base to 1s */
        RTC_SetPrescaler(32767);  
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}


uint8_t WaitWakeupPin()
{
    uint8_t wakup_pin_state = 0;
    
    TimingDelay = 100 * 3;
    
    /* Enable the SysTick Counter */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
    
    wakup_pin_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    
    
    while ((TimingDelay != 0 ) && (wakup_pin_state == 1))
    {
        wakup_pin_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    }
    
    /* Disable the SysTick Counter */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
    
    /* Clear the SysTick Counter */
    SysTick->VAL = (uint32_t)0x0;
    
    return wakup_pin_state;
}
void Board_ADC_Init()
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
    /* Configure PC.05,PC.04 as AIN*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    
    
    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* ADC1 regular channels configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_28Cycles5);    
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    
    
   /* ADC2 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC2, &ADC_InitStructure);
    /* ADC2 regular channels configuration */ 
    ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_28Cycles5);
    /* Enable ADC2 EOC interupt */
    ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);    
      /* Enable ADC2 */
    ADC_Cmd(ADC2, ENABLE);

    
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));
    
    
    /* Enable ADC2 */
    ADC_Cmd(ADC2, ENABLE);
    
    /* Enable ADC2 reset calibaration register */   
    ADC_ResetCalibration(ADC2);
    /* Check the end of ADC2 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC2));
    
    /* Start ADC2 calibaration */
    ADC_StartCalibration(ADC2);
    /* Check the end of ADC2 calibration */
    while(ADC_GetCalibrationStatus(ADC2));
    
}
void CheckVoltage()
{
    /* Check the voltage*/      
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    /*wait for the TC14 to be 1*/
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==0);
    DMA_ClearFlag(DMA1_FLAG_TC1);     
    
    /*The value is in ADC1ConvertedValue*/
    ADC1ConvertedValue = ADC1ConvertedValue;
    
}

uint16_t GetBatteryInfo()
{
  
      /* Check the voltage*/      
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    /*wait for the TC14 to be 1*/
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==0);
    DMA_ClearFlag(DMA1_FLAG_TC1);     
    
    /*The value is in ADC1ConvertedValue*/
    ADC1ConvertedValue = ADC1ConvertedValue;
    
    return ADC1ConvertedValue;
    

}
uint16_t GetTemperature()
{
    /* Check the voltage*/      
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);

    
    /*The value is in ADC1ConvertedValue*/
    ADC2ConvertedValue = ADC2ConvertedValue;
    
    return ADC2ConvertedValue;
    
    
}
void CheckPowerOnReason()
{
  
#if 0 
while(1)
{
Led_Both(4);
}

#endif 
  /*If there is usb connected, just return.*/
   USB_Plugin_State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
   if(USB_Plugin_State == 1)
   {
        Delay(15);
        USB_Plugin_State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);     
        if(USB_Plugin_State == 1)
        {
              /* usb wake up*/
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_ON);             
                return ;
        }
   }
   
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)    
    {
      
      
        /*First time power on .*/
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
        Delay(50 * 4);
        BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_OFF);    
        
        RCC_ClearFlag();
        /* power off standby*/
        PWR_EnterSTANDBYMode();

    }
      else
    {
        if (WaitWakeupPin() == 1) //user press the pin for 3 seconds.....
        {
            if(FLAG_POWER_ON == BKP_ReadBackupRegister(BKP_POWER_ON))
            {
                //Power off
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_OFF);  
                
                Led_Power_Off();                
                PWR_EnterSTANDBYMode();    
            }
            else
            {
                //Power on
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_ON);             
                Led_Power_On();                
                
            }
            
        }
        else 
        {
            if(FLAG_POWER_ON == BKP_ReadBackupRegister(BKP_POWER_ON))
            {
                //power on with rtc or wake up pin -- need to data logger
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_ON);             
                
            }
            else
            {
              
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_OFF); 
                //continue to power off  mode.
                PWR_EnterSTANDBYMode();    
                
            }
        }
      
    }
    
}


void Led_Power_On()
{
  Led_Both(4);
}

void Led_Power_Off()
{
 Led_One_By_One(4); 
}

void Led_One_By_One(uint8_t count)
{
    uint8_t i;
    for(i = 0;i<count;i++)
    {
        /*Wake up by rtc or wake up pin*/
        GPIO_SetBits(GPIOA, GPIO_Pin_1 );            
        GPIO_ResetBits(GPIOA, GPIO_Pin_2 );                                    
        Delay(50); 
        GPIO_SetBits(GPIOA, GPIO_Pin_2 );            
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 );
        Delay(50); 
    }
  
}

void Led_Both(uint8_t count)
{
    uint8_t i;
    for(i = 0;i<count;i++)
    {
        /*Wake up by rtc or wake up pin*/
        GPIO_SetBits(GPIOA, GPIO_Pin_1 |GPIO_Pin_2 );            
        Delay(50); 
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 |GPIO_Pin_2 );                            
        Delay(50); 
    }
  
}
void Test_Flash()
{
  
    uint8_t i = 0;
    uint16_t flash_flag = 0;
    
    flash_flag = *(uint16_t *)FLASH_READY_ADDRESS;
    
    if( flash_flag != FLAG_FLASH_READY)
    {
        FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
        //Erase the 32 page 32K
        for(i = 0;i< DATA_PAGES;i++)
        {
            FLASH_ErasePage(DATA_LOGGER_ADDRESS_START + i * 1024);
        }
        FLASH_ErasePage(FLASH_READY_ADDRESS);
        FLASH_ProgramHalfWord(FLASH_READY_ADDRESS , FLAG_FLASH_READY);

        FLASH_ErasePage(REOCRD_COUNT_ADDRESS);        
        FLASH_ProgramHalfWord(REOCRD_COUNT_ADDRESS , 0x0000);
        
        FLASH_Lock();

    } 

    flash_flag = *(uint16_t *)FLASH_READY_ADDRESS;    
    if( flash_flag != FLAG_FLASH_READY)
    {
        FLASH_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
        //Erase the 32 page 32K
        for(i = 0;i< DATA_PAGES;i++)
        {
            FLASH_ErasePage(DATA_LOGGER_ADDRESS_START + i * 1024);
        }
        FLASH_ErasePage(FLASH_READY_ADDRESS);
        FLASH_ProgramHalfWord(FLASH_READY_ADDRESS , FLAG_FLASH_READY);

        FLASH_ErasePage(REOCRD_COUNT_ADDRESS);        
        FLASH_ProgramHalfWord(REOCRD_COUNT_ADDRESS , 0x0000);
        
        FLASH_Lock();

    } 

    while(1)
    {
              record_count =   *(uint16_t *)REOCRD_COUNT_ADDRESS;
  
              FLASH_Unlock();
              FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
              FLASH_ProgramHalfWord(DATA_LOGGER_ADDRESS_START + record_count * 2, 0xABCD);
              //Erase first
              FLASH_ErasePage(REOCRD_COUNT_ADDRESS);
              //Update the count
              record_count = record_count + 1;
              
              FLASH_ProgramHalfWord(REOCRD_COUNT_ADDRESS , record_count);
              FLASH_Lock();
      
      Delay(3);
    }
}
void Enable_SDcard()
{
         GPIO_SetBits(GPIOA, GPIO_Pin_3 );             
}

void Disable_SDcard()
{
         GPIO_ResetBits(GPIOA, GPIO_Pin_3 );             
}