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
static __IO uint32_t LedShowStatus = 0;
static __IO ErrorStatus HSEStartUpStatus = SUCCESS;
static __IO uint32_t SELStatus = 0;
static __IO uint8_t USBPlugin = 0;

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

__IO uint16_t ADC1ConvertedValue = 0, ADC3ConvertedValue = 0;


FIL  g_file_datalogger;
void RTC_Configuration_xp(void);
#define BKP_POWER_ON BKP_DR1
#define BKP_FLASH_READY BKP_DR2
#define BKP_DATA_LOGGER_COUNT BKP_DR3

#define FLAG_POWER_ON 0x5A5A
#define FLAG_POWER_OFF 0xC5B5

#define FLAG_FLASH_READY 0x7C8A
#define FLAG_FLASH_NOT_READY 0x0000

#define DATA_LOGGER_ADDRESS_START  ((uint32_t)0x08008000)

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
    /* Initialize the Demo */
    Demo_Init();
    
}

/*******************************************************************************
* Function Name  : Demo_Init
* Description    : Initializes the demonstration application.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Demo_Init(void)
{
    
    uint32_t record_count;
    uint8_t i;
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
    
    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
    
    /*------------------- Resources Initialization -----------------------------*/
    /* GPIO Configuration */
    GPIO_Config();
    
    /* Interrupt Configuration */
    InterruptConfig();
    
    /* Configure the systick */    
    SysTick_Configuration();
    
    /*------------------- Drivers Initialization -------------------------------*/
    /* Initialize the LEDs toogling */
    LedShow_Init();
    
    /* Initialize the Low Power application */
    LowPower_Init();
    
    
    
    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);
    
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    
    
    RTC_Configuration_xp();  
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    
    
    /* If HSE is not detected at program startup */
    if(HSEStartUpStatus == ERROR)
    {
        /* Generate NMI exception */
        SCB->ICSR |= SCB_ICSR_NMIPENDSET;
    }  
    
    
    
    WakupPin_Init();
    
    CheckPowerOnReason();
    
    
    
    My_ADC_Init();
    CheckVoltage();
    
    /*init the flag*/  
    if(BKP_ReadBackupRegister(BKP_FLASH_READY) != FLAG_FLASH_READY)
    {
        FLASH_Unlock();
        
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
        
        BKP_WriteBackupRegister(BKP_FLASH_READY, FLAG_FLASH_READY);
        for(i = 0;i< 32;i++)
        {
            FLASH_ErasePage(DATA_LOGGER_ADDRESS_START + i * 1024);
        }
        BKP_WriteBackupRegister(BKP_DATA_LOGGER_COUNT, 0x0000); 
        FLASH_Lock();
    }
    
    
    USBPlugin = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    
    if(USBPlugin == 0)
    {
        
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        GPIO_ResetBits(GPIOA,GPIO_Pin_2);
        Delay(50);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        GPIO_SetBits(GPIOA,GPIO_Pin_2);  
        
        record_count =   BKP_ReadBackupRegister(BKP_DATA_LOGGER_COUNT);
        
        if(record_count >= 15428)
        {
            //Write the full.
        }
        else
        {
            FLASH_Unlock();
            FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	        
            FLASH_ProgramHalfWord(DATA_LOGGER_ADDRESS_START + record_count * 2, GetTemperature());
            FLASH_Lock();
            BKP_WriteBackupRegister(BKP_DATA_LOGGER_COUNT, record_count + 1);  			
        }
        
        
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        /*
        if there is usb connect, copy the data to sdcard. and start the mass storage
        */
        NAND_FAT();  
        CreateDataLoggerFile();    
        Mass_Storage_Start ();     
        while(bDeviceState != CONFIGURED);
        while(bDeviceState == CONFIGURED);
        PowerOff();    
        /* Generate a system reset */  
        NVIC_SystemReset();    
    }
    
    
    /* Wait till RTC Second event occurs */
    RTC_ClearFlag(RTC_FLAG_SEC);
    while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);
    
    /* Set the RTC Alarm after 10s */
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
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
    
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* Enable the EXTI3 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the EXTI9_5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the EXTI15_10 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    
    
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
    
    /* Enable the EXTI Line17 Interrupt */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
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
* Function Name  : IntExtOnOffConfig
* Description    : Enables or disables EXTI for the menu navigation keys :
*                  EXTI lines 3, 7 and 15 which correpond respectively
*                  to "DOWN", "SEL" and "UP".
* Input          : NewState: New state of the navigation keys. This parameter
*                  can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void IntExtOnOffConfig(FunctionalState NewState)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Initializes the EXTI_InitStructure */
    EXTI_StructInit(&EXTI_InitStructure);
    
    /* Disable the EXTI line 3, 7 and 15 on falling edge */
    if(NewState == DISABLE)
    {
        EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line7 | EXTI_Line15;
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
        EXTI_Init(&EXTI_InitStructure);
    }
    /* Enable the EXTI line 3, 7 and 15 on falling edge */
    else
    {
        /* Clear the the EXTI line 3, 7 and 15 interrupt pending bit */
        EXTI_ClearITPendingBit(EXTI_Line3 | EXTI_Line7 | EXTI_Line15);
        
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line7 | EXTI_Line15;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
    }
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
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
* Function Name  : DelayJoyStick
* Description    : Inserts a delay time while no joystick RIGHT, LEFT and SEL 
pushbuttons are pressed.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : Pressed Key.  This value can be: NOKEY, RIGHT, LEFT or SEL.
*******************************************************************************/
uint32_t DelayJoyStick(__IO uint32_t nTime)
{
    __IO uint32_t keystate = 0;
    
    /* Enable the SysTick Counter */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
    
    TimingDelay = nTime;
    
    while((TimingDelay != 0) && (keystate != RIGHT) && (keystate != LEFT) && (keystate != SEL))
    {
        keystate = ReadKey();
    } 
    
    return keystate;
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

/*******************************************************************************
* Function Name  : Set_SELStatus
* Description    : Sets the SELStatus variable.
* Input          : None
* Output         : SELStatus
* Return         : None
*******************************************************************************/
void Set_SELStatus(void)
{
    SELStatus = 1;
}

void WakupPin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* PA.01, PA.02 as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
}


/*******************************************************************************
* Function Name  : LedShow_Init
* Description    : Configure the leds pins as output pushpull: LED1, LED2, LED3
*                  and LED4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LedShow_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* PA.01, PA.02 as output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOF, &GPIO_InitStructure); 
    
}

/*******************************************************************************
* Function Name  : LedShow
* Description    : Enables or disables LED1, LED2, LED3 and LED4 toggling.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LedShow(FunctionalState NewState)
{
}

/*******************************************************************************
* Function Name  : Get_LedShowStatus
* Description    : Get the LedShowStatus value.
* Input          : None
* Output         : None
* Return         : LedShowStatus Value.
*******************************************************************************/
uint32_t Get_LedShowStatus(void)
{
    return LedShowStatus;
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
    
    uint16_t record_count = 0;
    uint8_t i = 0;    
    uint16_t value = 0;  
    record_count =   BKP_ReadBackupRegister(BKP_DATA_LOGGER_COUNT);
    /* Create destination file on the drive 0 */
    res = f_open(&g_file_datalogger, "0:datalogger.bin", FA_CREATE_ALWAYS | FA_WRITE);
    if (res) die(res);
    
    for(i =0;i<record_count;i++)
    {
        value = *(uint16_t*)(DATA_LOGGER_ADDRESS_START + i * 2);
        res = f_write(&g_file_datalogger, &value, sizeof(value), &bw);
    }
    f_close(&g_file_datalogger);
    
}

void RTC_Configuration_xp(void)
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
void My_ADC_Init()
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
    
    
    
    
    /* DMA2 channel5 configuration ----------------------------------------------*/
    DMA_DeInit(DMA2_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC3ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);  
    /* Enable DMA2 channel5 */
    DMA_Cmd(DMA2_Channel5, ENABLE);
    
    
    
    
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
    
    
    /* ADC3 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC3, &ADC_InitStructure);
    /* ADC3 regular channel configuration */ 
    ADC_RegularChannelConfig(ADC3, ADC_Channel_15, 1, ADC_SampleTime_28Cycles5);
    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);
    
    
    
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
    
    
    
    
    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);
    
    /* Enable ADC3 reset calibaration register */   
    ADC_ResetCalibration(ADC3);
    /* Check the end of ADC3 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC3));
    
    /* Start ADC3 calibaration */
    ADC_StartCalibration(ADC3);
    /* Check the end of ADC3 calibration */
    while(ADC_GetCalibrationStatus(ADC3));
    
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
uint16_t GetTemperature()
{
    /* Check the voltage*/      
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    
    /*wait for the TC5 to be 1*/
    while(DMA_GetFlagStatus(DMA2_FLAG_TC5)==0);
    DMA_ClearFlag(DMA2_FLAG_TC5);
    
    /*The value is in ADC1ConvertedValue*/
    ADC3ConvertedValue = ADC3ConvertedValue;
    
    return ADC3ConvertedValue;
    
    
}
void CheckPowerOnReason()
{
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
        /*First time power on .*/
        GPIO_SetBits(GPIOF, GPIO_Pin_6 |GPIO_Pin_7 );
        GPIO_ResetBits(GPIOF, GPIO_Pin_8 |GPIO_Pin_9 );
        Delay(50 * 1);
        GPIO_ResetBits(GPIOF, GPIO_Pin_6 |GPIO_Pin_7 );
        GPIO_SetBits(GPIOF, GPIO_Pin_8 |GPIO_Pin_9 );
        Delay(50 * 1);
        RCC_ClearFlag();
        /* power off standby*/
        PWR_EnterSTANDBYMode();
    }
    else
    {
        /*Wake up by rtc or wake up pin*/
        GPIO_SetBits(GPIOF, GPIO_Pin_6 |GPIO_Pin_9 );            
        
        if (WaitWakeupPin() == 1) //user press the pin for 3 seconds.....
        {
            if(FLAG_POWER_ON == BKP_ReadBackupRegister(BKP_POWER_ON))
            {
                //Power off
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_OFF);    
                PWR_EnterSTANDBYMode();    
                
            }
            else
            {
                //Power on
                BKP_WriteBackupRegister(BKP_POWER_ON, FLAG_POWER_ON);             
                GPIO_SetBits(GPIOF, GPIO_Pin_6 |GPIO_Pin_7| GPIO_Pin_8 |GPIO_Pin_9 );                                           
                Delay(100 * 2);                   
            }
            
        }
        else 
        {
            if(FLAG_POWER_ON == BKP_ReadBackupRegister(BKP_POWER_ON))
            {
                //power on with rtc or wake up pin -- need to data logger
                
            }
            else
            {
                //continue to standby mode.
                PWR_EnterSTANDBYMode();    
                
            }
        }
    }
    
}