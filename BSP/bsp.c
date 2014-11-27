/*
*********************************************************************************************************
*                               UIF BOARD APP PACKAGE
*
*                            (c) Copyright 2013 - 2016; Fortemedia Inc.; Nanjing, China
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*
*                                       BOARD SUPPORT PACKAGE
*
*                                          Atmel AT91SAM7A3
*                                               on the
*                                      Unified EVM Interface Board
*
* Filename      : bsp.c
* Version       : V1.0.0
* IDE           : IAR for ARM 5.40
* Programmer(s) : PQ
*
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE

#include  <includes.h>


/*
*********************************      Version Declaration       ****************************************
*/
const CPU_CHAR fw_version[]  = "[FW:H:V0.4]"; //fixed size string

#ifdef  BOARD_TYPE_AB01
const CPU_CHAR hw_version[]  = "[HW:V1.0]"; 
const CPU_CHAR hw_model[]    = "[AB01]"; 
#endif
#ifdef BOARD_TYPE_AB02
const CPU_CHAR hw_version[]  = "[HW:V1.0]"; 
const CPU_CHAR hw_model[]    = "[AB02]"; 
#endif
#ifdef  BOARD_TYPE_AB03
const CPU_CHAR hw_version[]  = "[HW:V1.0]"; 
const CPU_CHAR hw_model[]    = "[AB03]"; 
#endif
#ifdef  BOARD_TYPE_UIF
const CPU_CHAR hw_version[]  = "[HW:V1.0]"; 
const CPU_CHAR hw_model[]    = "[UIF01]"; 
#endif



OS_EVENT *Bsp_Ser_Tx_Sem_lock;  
OS_EVENT *Bsp_Ser_Rx_Sem_lock;

static  Pin PinBuzzer    =   BUZZER ;
CPU_INT08U Debug_COM_Sel = 0 ; //debug uart use:    0: DBGUART, 1: UART1, >1: debug muted

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_RAM_REMAP_TEST_BYTE             (*(CPU_INT08U  *)0x00000030L)

#define  BSP_GPIOA_TWD                      DEF_BIT_00           /* TWI   (A): Serial EEPROM                                 */
#define  BSP_GPIOA_TWCK                     DEF_BIT_01           /* TWI   (A): Serial EEPROM                                 */                                                                /* ---------------------- GPIOA Pins ---------------------- */
#define  BSP_GPIOA_RXD0                     DEF_BIT_02           /* UART0 (A): COM PORT 0                                    */
#define  BSP_GPIOA_TXD0                     DEF_BIT_03           /* UART0 (A): COM PORT 0                                    */                                 
#define  BSP_GPIOA_UART0_SEL                DEF_BIT_04          /* UART0 (A): COM PORT 0                                    */
#define  BSP_GPIOA_RXD1                     DEF_BIT_07          /* UART0 (A): Debug Port                                    */
#define  BSP_GPIOA_TXD1                     DEF_BIT_08          /* UART0 (A): Debug Port                                    */
#define  BSP_GPIOA_RXD2                     DEF_BIT_09          /* UART0 (A): Debug Port                                    */
#define  BSP_GPIOA_TXD2                     DEF_BIT_10          /* UART0 (A): Debug Port     */
#define  BSP_GPIOA_UART2_SEL                DEF_BIT_11          /* UART0 (A): COM PORT 0                                    */
#define  BSP_GPIOA_SPI0_NPCS3               DEF_BIT_14          /* SPI0  (A): DataFlash Chip Select                         */
#define  BSP_GPIOA_SPI0_MISO                DEF_BIT_15          /* SPI0  (A): SD/MMC/DataFlash Socket/DataFlash             */
#define  BSP_GPIOA_SPI0_MOSI                DEF_BIT_16          /* SPI0  (A): SD/MMC/DataFlash Socket/DataFlash             */
#define  BSP_GPIOA_SPI0_SPCK                DEF_BIT_17          /* SPI0  (A): SD/MMC/DataFlash Socket/DataFlash             */
#define  BSP_GPIOA_DRXD                     DEF_BIT_30          /* DBGU  (A): Debug Port                                    */
#define  BSP_GPIOA_DTXD                     DEF_BIT_31          /* DBGU  (A): Debug Port                                    */
#define  BSP_GPIOA_BUZZER                   DEF_BIT_29

////////////////  ????
#define  BSP_GPIOA_CAN_RXD                  DEF_BIT_19          /* CAN   (A): CAN                                           */
#define  BSP_GPIOA_CAN_TXD                  DEF_BIT_20          /* CAN   (A): CAN                                           */
#define  BSP_GPIOA_JOY_UP                   DEF_BIT_21          /* (GPIO)(A): Joystick Up                                   */
#define  BSP_GPIOA_JOY_DOWN                 DEF_BIT_22          /* (GPIO)(A): Joystick Down                                 */
#define  BSP_GPIOA_JOY_LEFT                 DEF_BIT_23          /* (GPIO)(A): Joystick Left                                 */
#define  BSP_GPIOA_JOY_RIGHT                DEF_BIT_24          /* (GPIO)(A): Joystick Right                                */
#define  BSP_GPIOA_JOY_PUSH                 DEF_BIT_25          /* (GPIO)(A): Joystick Push                                 */
////////////////

#define  BSP_GPIOA_DBG                     (BSP_GPIOA_DRXD      |\
                                            BSP_GPIOA_DTXD)

#define  BSP_GPIOA_UART0                   (BSP_GPIOA_RXD0      |\
                                            BSP_GPIOA_TXD0)

#define  BSP_GPIOA_UART1                   (BSP_GPIOA_RXD1      |\
                                            BSP_GPIOA_TXD1)

#define  BSP_GPIOA_JOY                     (BSP_GPIOA_JOY_UP    | \
                                            BSP_GPIOA_JOY_DOWN  | \
                                            BSP_GPIOA_JOY_LEFT  | \
                                            BSP_GPIOA_JOY_RIGHT | \
                                            BSP_GPIOA_JOY_PUSH)

                                                                /* ---------------------- GPIOB Pins ---------------------- */
#define  BSP_GPIOA_LED1                     DEF_BIT_12          /* (GPIO)( ): LED 1                                         */
#define  BSP_GPIOA_LED2                     DEF_BIT_13          /* (GPIO)( ): LED 2                                         */


#define  BSP_GPIOA_LEDS                    (BSP_GPIOA_LED1      | \
                                            BSP_GPIOA_LED2)
                                                                /* --------------- TIMEOUT VALUES ------------------- */
#define  BSP_PLL_MAX_TIMEOUT                    0xFFFF
#define  BSP_MAIN_OSC_MAX_START_UP_VAL          0xFFFF
#define  BSP_MAIN_OSC_MAINF_MAX_TIMEOUT_VAL     0xFFFF
#define  BSP_MCLK_MAX_TIMEOUT                   0xFFFF
                                                                /* ------------ PERIPHERALS CONSTRAINS  ------------- */
#define  BSP_PLL_MAX_MUL                          2048          /* Maximum value for the PLL Multiplier               */
#define  BSP_AIC_INT_PRIO_MAX                        7          /* Highest Interrupt level in the AIC                 */
#define  BSP_PER_ID_MAX                             31          /* Number of peripherals.                             */


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_DummyISR_Handler  (void);

static  void  BSP_IntInit           (void);

static  void  BSP_Tmr_TickInit      (void);

static  void  BSP_ResetInit         (void);
/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOABL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
  
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;       /* Disable the Watchdog Timer                               */
    
    BSP_IntInit();                                       /* Initialize the Interrupt Controller, must put here front */
 
    GPIO_Init();   
       
    BSP_ResetInit();                                     /* Enable the hardware reset button  used interrupt         */
         
    // Configure IIC    
    TWI_Init( TWCK ); //It seems that the TWI will auto desrease SCK if loading increase           
    
    //Configure SPI in default mode
    SPI_Init( SPI_CLK, 0 ) ; 
    
    // Init Internal Flash
    FLASHD_Initialize( MCK );
    //flash_test();
    
    // Config USART
    UART_Init(PC_UART,     ISR_PC_UART,     115200 );    //To PC        
    //UART_Init(RULER_UART,  ISR_Ruler_UART,  115200 );    //To Ruler 
    UART_Init(AUDIO_UART,  NULL,            115200 );    //To Audio  
          
    //Config Timer
    Timer_Init();  
    
    //init noah CMD control varieties 
    //Init_Noah_Mem_Ctr();
    
    //Init ADCs
    //Init_ADC_Voltage() ; 
    //Init_ADC_Current() ;   
    
    BSP_Tmr_TickInit();                                         /* Initialize uC/OS-II's Tick Rate and DEBUG UART                          */
    

}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Get the processor clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  mclk_css;
    CPU_INT32U  mclk_div;
    CPU_INT32U  cpu_freq;

    
    mclk_css = (AT91C_BASE_PMC->PMC_MCKR     ) & 0x00000003;    /* Get the master clk source                          */
    mclk_div = (AT91C_BASE_PMC->PMC_MCKR >> 2) & 0x00000003;    /* Get the master clk preescaler                      */
    mclk_div = DEF_BIT(mclk_div);
    
    switch (mclk_css) {

        case BSP_CLK_SRC_SLOW:                                 /* Slow clock                                         */
             cpu_freq = BSP_SLOW_XTAL_FREQ;
             break;

        case BSP_CLK_SRC_MAIN:                                 /* Main clock                                         */
             cpu_freq = BSP_MAIN_XTAL_FREQ;
             break;

        case BSP_CLK_SRC_PLL:                                  /* PLL clock                                          */
             cpu_freq = BSP_PLL_GetFreq();
             break;

        case BSP_CLK_SRC_RESERVED:
        default:
             cpu_freq = 0;
             break;             
    }

    cpu_freq = cpu_freq / mclk_div;

    return (cpu_freq);
}

/*
*********************************************************************************************************
*                                          BSP_CPU_Init()
*
* Description : Initializes all the CPU clk.
*               (1) Enable the Main Oscillator.
*               (2) Set the PLL at 192 Mhz:
*                     PLL_CLK = (MAINclk * mult) / div  = 18432000 * (72 + 1) / 7
*                                                       = 192219428.6 Mhz ; mult = 73, div = 7 (0.11 % err for 192 Mhz).
*               (3) Set the MCLK/CPUClk/PCK frequency.
*                   MCLK = PCLK / div                   = 192  / 4 = 48 Mhz.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init();
*
* Note(s)     : (1) From the AT91SAM7x datasheet 
*                   (http://www.atmel.com/dyn/resources/prod_documents/doc6120.pdf)
*                   The PLL has the following characteristics.
*
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Parameter        |   Condition                     |  MIN  |  MAX  |  Unit  ||
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Output Frequency | If OUT field in CKGR_PLL is 00  |   80  |  160  |  Mhz   ||
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Output Frequency | If OUT field in CKGR_PLL is 01  |   150 |  200  |  Mhz   ||
*                 ||-----------------------------------------------------------------------------||
*
*********************************************************************************************************
*/

void  BSP_CPU_Init (void)
{
    AT91C_BASE_MC->MC_FMR      |= (1 << 8); 
    BSP_MainOscEn( 0xFF );                                        /* Enable Main Oscillator                            */
    
    // 18.432 * 73 / 7 =  192.2MHz   
    BSP_PLL_Cfg( 73,                                              /* PLL Multipler                                     */
                 7,                                               /* PLL Divider                                       */
                 1,                                               /* PLL Out parameter (see note #1)                   */ 
                 0xFF );                                          /* Set the PLL frequency                             */

    BSP_MclkCfg( BSP_CLK_SRC_PLL,
                 BSP_CLK_PRE4 );                                  /* MCK = 192M / 4 = 48MHz                            */
                 
}




/*
*********************************************************************************************************
*                                    void LowLevelInitPLL(void)  
*                    In order to boost powerup speed, init PLL first in cstartup.s !!!!!
*                                          by PQ
*
*********************************************************************************************************
*/
// Startup time of main oscillator (in number of slow clock ticks).
#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (0x40 << 8))
// USB PLL divisor value to obtain a 48MHz clock.
#define BOARD_USBDIV            AT91C_CKGR_USBDIV_1
// PLL frequency range.
#define BOARD_CKGR_PLL          AT91C_CKGR_OUT_0
// PLL startup time (in number of slow clock ticks).
#define BOARD_PLLCOUNT          (16 << 8)

// PLL MUL value.
#define BOARD_MUL               (AT91C_CKGR_MUL & (72 << 16))   //96M     = 18.432 * (72+1) / 14 = 96.1 
//#define BOARD_MUL               (AT91C_CKGR_MUL & (83 << 16))  //110.592M = 18.432 * (83+1) / 14 = 110.592
//#define BOARD_MUL               (AT91C_CKGR_MUL & (90 << 16))   //140M  = 18.432 * (90+1) / 12 = 139.77
// PLL DIV value.
#define BOARD_DIV               (AT91C_CKGR_DIV & 14)
//#define BOARD_DIV               (AT91C_CKGR_DIV & 12)

// Master clock prescaler value.
#define BOARD_PRESCALER         AT91C_PMC_PRES_CLK_2

 
void LowLevelInitPLL( void )
{  
    // Set flash wait states in the EFC
    // 48MHz = 1 wait state
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_1FWS;

    // Initialize main oscillator
    AT91C_BASE_PMC->PMC_MOR = BOARD_OSCOUNT | AT91C_CKGR_MOSCEN;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

    // Initialize PLL at 96MHz (96.109) and USB clock to 48MHz
    AT91C_BASE_PMC->PMC_PLLR = BOARD_USBDIV | BOARD_CKGR_PLL | BOARD_PLLCOUNT
                               | BOARD_MUL | BOARD_DIV;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));
        

    // Wait for the master clock if it was already initialized
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Switch to slow clock + prescaler
    AT91C_BASE_PMC->PMC_MCKR = BOARD_PRESCALER;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Switch to fast clock + prescaler
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));    

}


/*
*********************************************************************************************************
*********************************************************************************************************
                                    INTERRUPT CONTROLLER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          BSP_IntEn()
*
* Description : This function clears an interrupt
*
* Argument(s) : per_id        The interrupt id
*                             BSP_PER_ID_AIC_FIQ
*                             BSP_PER_ID_SYSC
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_IntClr (CPU_INT08U  per_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

    AT91C_BASE_AIC->AIC_ICCR = DEF_BIT(per_id);
}

/*
*********************************************************************************************************
*                                             BSP_IntDis()
*
* Description : This function disables an interrupt.
*
* Argument(s) : per_id        The interrupt id
*                             BSP_PER_ID_AIC_FIQ
*                             BSP_PER_ID_SYSC
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_IntDis (CPU_INT08U  per_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

    AT91C_BASE_AIC->AIC_IDCR = DEF_BIT(per_id); 
}

/*
*********************************************************************************************************
*                                             BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF;                      /* Disable all interrupts                                 */
}

/*
*********************************************************************************************************
*                                          BSP_IntEn()
*
* Description : This function enables an interrupt.
*
* Argument(s) : per_id        The interrupt id
*                             BSP_PER_ID_AIC_FIQ
*                             BSP_PER_ID_SYSC
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_IntEn (CPU_INT08U  per_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

    AT91C_BASE_AIC->AIC_IECR = DEF_BIT(per_id);
}


/*
*********************************************************************************************************
*                                       BSP_IntInit()
*
* Description : (1) Initialize interrupts:
*                    (a) Initialize exception vector table, performing a remap (if necessary).
*                    (b) Initialize vectors in Advance Interrupt Controller (AIC) to dummy ISR.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_IntInit (void)
{
    CPU_INT16U  i; 
    
    BSP_RAM_REMAP_TEST_BYTE = 0xAA;                             /* Write a byte to RAM   */
    if (BSP_RAM_REMAP_TEST_BYTE != 0xAA) {                      /* Check if the write to RAM worked v                       */
        AT91C_BASE_MC->MC_RCR =  1;                             /* If so, toggle REMAP register                             */
    }

    AT91C_BASE_AIC->AIC_ICCR  =  AT91C_BASE_AIC->AIC_ISR;       /* Clear the current interrupt (if any pending)             */
    AT91C_BASE_AIC->AIC_EOICR =  0x00000000;                    /* End-of-interrupt command                                 */
    
    for (i = 0; i < 32; i++) {                                  /* Assign dummy ISR                                         */
         BSP_IntVectSet ((CPU_INT08U   )i,
                         (CPU_INT08U   )0,
                         (CPU_INT08U   )BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE,
                         (CPU_FNCT_VOID)BSP_DummyISR_Handler);
            
    }

    AT91C_BASE_AIC->AIC_SPU   = (CPU_INT32U)0;
    AT91C_BASE_AIC->AIC_DCR = DEF_BIT_00;
}


/*
*********************************************************************************************************
*                                       BSP_ResetInit()
*
* Description : (1) Initialize NRST interruption:
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  BSP_ResetInit (void)
{   
  
   AT91C_BASE_RSTC->RSTC_RMR = (CPU_INT32U)(0xA5 << 24) | AT91C_RSTC_URSTIEN ;  //enable NRST interrupt    
   AT91C_BASE_RSTC->RSTC_RSR; // read RSR, clear previous data   
   
}


/*
*********************************************************************************************************
*                                          BSP_IntVectSet()
*
* Description : This function sets the specified interrupt source's ISR handler call back function.
*
* Argument(s) : per_id          The interrupt id
*                               BSP_PER_ID_AIC_FIQ
*                               BSP_PER_ID_SYSC
*                               BSP_PER_ID_PIOA
*                               BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                               BSP_PER_ID_AIC
*
*               int_prio        Interrupt priority level. 0(lowest) - 7 (highest).
*
*               int_src_type    Interrupt source type.
*                               BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE
*                               BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG
*                               BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE
*                               BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE
*                               BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG
*                               BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG
*                                                
*               int_isr         ISR handler function in the form of 'void ISR_Handler (void)'.
*
* Returns    : none
*********************************************************************************************************
*/
void  BSP_IntVectSet (CPU_INT08U     per_id,
                      CPU_INT08U     int_prio,
                      CPU_INT08U     int_src_type,
                      CPU_FNCT_VOID  int_isr_fnct)
{
    CPU_INT08U  aic_scr_type;
                      
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */
    if (int_prio > BSP_AIC_INT_PRIO_MAX) {
        return;
    }
    
    if (int_isr_fnct == (CPU_FNCT_VOID)0 ) {
        return;
    }
 
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

                                                                /* Set the interrupt vector with the ISR handler      */
    AT91C_BASE_AIC->AIC_SVR[per_id] = (CPU_INT32U)int_isr_fnct;
    
    switch (int_src_type) {
        case BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE:
        case BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE:
             aic_scr_type = 0x00;
             break;
    
        case BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG:
        case BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG:
             aic_scr_type = 0x01;
             break;

        case BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE:
             aic_scr_type = 0x02;
             break;

        case BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG:
             aic_scr_type = 0x03;
             break;

        default:
             aic_scr_type = 0x00;
             break;             
    }
                                                          
    AT91C_BASE_AIC->AIC_SMR[per_id] = (int_prio         )       /* Set Interrupt priority and type                    */
                                    | (aic_scr_type << 5);
}

/*
*********************************************************************************************************
*                                        BSP_DummyISR_Handler()
*
* Description : Handle invalid IRQs.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  BSP_DummyISR_Handler (void)
{
    CPU_INT32U  irq_id;


    AT91C_BASE_AIC->AIC_IVR  = 0;                               /* Debug variant of vector read                       */
    irq_id                   = AT91C_BASE_AIC->AIC_ISR & 0x1F;  /* Retrieve the ID of the interrupting source         */
    BSP_IntClr(irq_id);                                         /* Clear the current interrupt                        */
                              
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/







/*
*********************************************************************************************************
*********************************************************************************************************
**                                       MAIN OSCILLATOR FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_MainOscDis()
*
* Description : Disable the Main Oscillator.
*
* Argument(s) : none.
*
* Return(s)   : none.
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void BSP_MainOscDis (void)
{
    DEF_BIT_CLR(AT91C_BASE_PMC->PMC_MOR, DEF_BIT_00);
}            


/*
*********************************************************************************************************
*                                           BSP_MainOscEn()
*
* Description : Enable the Main Oscillator.
*
* Argument(s) : start_time    Specifies the number of slow clock cycles multiply by 8 
*                             for the main oscillator start-up time.
*
* Return(s)   : DEF_TRUE      If the main oscillator is enabled and stabilized
*               DEF_FALSE     If the main oscillator could not be stabilized during a period of time
*                             specified by BSP_MAIN_OSC_MAX_START_UP_VAL
* 
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_MainOscEn (CPU_INT08U  start_time)
{
    CPU_INT32U  timeout;
    
   
    AT91C_BASE_PMC->PMC_MOR = DEF_BIT_00
                            | (start_time << 8);

    timeout                 = BSP_MAIN_OSC_MAX_START_UP_VAL;
    
    while ((timeout > 0) && 
           (DEF_BIT_IS_CLR(AT91C_BASE_PMC->PMC_SR, DEF_BIT_00))) {
        timeout--;       
    }
           
    if (timeout == 0) {
        return (DEF_TRUE);
    } else {
        return (DEF_FALSE);
    }        
}
                        
/*
*********************************************************************************************************
*                                        BSP_MainOscGetFreq()
*
* Description : Return the Frequency of the main oscillator.
*
* Argument(s) : none
*               
*
* Return(s)   : The frequency of the Main Oscillator.
*                               
* Caller(s)   : Application.
*
* Note(s)     : (1) The Main Oscillator features a Main Clock frequency counter that provides the quartz frequency
*                   connected to the Main Oscillator. Generally, this value is known by the system designer; however,
*                   it is useful for the boot program to configure the device with the correct clock speed,
*                   independently of the application.
*
*               (2) This functions should not be used  unless the current value of the RC oscillator frequency is know.
*                   Since th RC oscillator characteristics may change (capacitancy value, etc) the value of BSP_SLOW_XTAL_FREQ needs 
*                   to be changed.
*********************************************************************************************************
*/

CPU_INT32U  BSP_MainOscGetFreq (void)
{
    CPU_INT32U  timeout;
    CPU_INT16U  main_clk_ctr;
    CPU_INT32U  freq;
    
    
    timeout = BSP_MAIN_OSC_MAINF_MAX_TIMEOUT_VAL;
    
    while ((timeout > 0) && 
           (DEF_BIT_IS_CLR(AT91C_BASE_PMC->PMC_MCFR, DEF_BIT_16))) {
        timeout--;       
    }

    if (timeout == 0) {
        freq = 0;
    } else {
        main_clk_ctr = (AT91C_BASE_PMC->PMC_MCFR & 0xFFFF);
        freq         = (main_clk_ctr * BSP_SLOW_XTAL_FREQ) / 16;
    }    
    
    return (freq);
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                          MASTER CLK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_MclkGetFreq()
*
* Description : Get the master clock frequency (MCK).
*
* Argument(s) : none.
*
* Return(s)   : The master clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_MclkGetFreq (void)
{
    CPU_INT32U  mclk_freq;
    CPU_INT08U  mclk_div;
    CPU_INT32U  mclk_src_freq;
    CPU_INT08U  mclk_src;

    
    mclk_src = (AT91C_BASE_PMC->PMC_MCKR & 0x03);
    
    switch (mclk_src) {
        case BSP_CLK_SRC_SLOW:
             mclk_src_freq = BSP_SLOW_XTAL_FREQ;
             break;
    
        case BSP_CLK_SRC_MAIN:
             mclk_src_freq = BSP_MAIN_XTAL_FREQ; 
             break;

        case BSP_CLK_SRC_PLL:
             mclk_src_freq = BSP_PLL_GetFreq(); 
             break;

        default:
             mclk_src_freq = 0; 
             break;
             
    }

    mclk_div  = ((AT91C_BASE_PMC->PMC_MCKR >> 2) & 0x03);       /* Read the Master Clock preescaler                   */
    mclk_div  = DEF_BIT(mclk_div);
    mclk_freq = (mclk_src_freq / mclk_div);

    return (mclk_freq);
}

/*
*********************************************************************************************************
*                                            BSP_MclkCfg()
*
* Description : This function configure the Master clk and the Processor clk
*
* Argument(s) : clk_scr      Selects the Master clk source:
*                            BSP_MCLK_SRC_SLOW      
*                            BSP_MCLK_SRC_MAIN      
*                            BSP_MCLK_SRC_PLL
*   
*
*               cpu_clk_pre  The processor clk preescaler
*                            BSP_CLK_PRE1   Selected clock
*                            BSP_CLK_PRE2   Selected clock divided by  2
*                            BSP_CLK_PRE4   Selected clock divided by  4
*                            BSP_CLK_PRE8   Selected clock divided by  8
*                            BSP_CLK_PRE16  Selected clock divided by 16
*                            BSP_CLK_PRE32  Selected clock divided by 32
*                            BSP_CLK_PRE64  Selected clock divided by 64
*   
* Return(s)   : DEF_TRUE      If the master clk is ready
*               DEF_FALSE     If the main oscillator could not be set in BSP_MCLK_MAX_TIMEOUT cycles
*                             or if a the paramater are incorrect
*                             
* 

* Return(s)   : none. 
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_MclkCfg (CPU_INT08U  mclk_src,
                          CPU_INT08U  cpu_clk_pre)

{
    CPU_INT32U  timeout;
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
    if ((mclk_src != BSP_CLK_SRC_SLOW) &&
        (mclk_src != BSP_CLK_SRC_MAIN) &&
        (mclk_src != BSP_CLK_SRC_PLL)) {
        return (DEF_FALSE);
    }
    
    if (cpu_clk_pre > BSP_CLK_PRE64) {
        return (DEF_FALSE);
    }
 
    AT91C_BASE_PMC->PMC_MCKR = ((mclk_src    & 0x03) << 0)
                             | ((cpu_clk_pre & 0x07) << 2);
    
   timeout = BSP_MCLK_MAX_TIMEOUT;

    while ((timeout > 0) && 
          (DEF_BIT_IS_CLR(AT91C_BASE_PMC->PMC_SR, DEF_BIT_03))) {
        timeout--;       
    }
        
    if (timeout == 0) {
        return (DEF_FALSE);
    } else {
        return (DEF_TRUE);  
    }

}



/*
*********************************************************************************************************
*********************************************************************************************************
**                                          PERIPHERAL CLK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP_PerClkDis()
*
* Description : Disable the corresponding peripheral clock
*
* Argument(s) : per_id        The peripheral id to be disabled (see note #1)
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1). The peripheral clk id are defined in the bsp.h file.
*********************************************************************************************************
*/

void  BSP_PerClkDis (CPU_INT08U  per_id)
{
                                                                /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return;
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return;
    }
    
    AT91C_BASE_PMC->PMC_PCDR = DEF_BIT(per_id);
}

/*
*********************************************************************************************************
*                                            BSP_PerClkEn()
*
* Description : Enable the corresponding peripheral clock
*
* Argument(s) : per_id        The peripheral id to be enabled (see note #1)
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1). The peripheral clk id are defined in the bsp.h file.
*********************************************************************************************************
*/

void  BSP_PerClkEn (CPU_INT08U per_id)
{
                                                                /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return;
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return;
    }
 
    AT91C_BASE_PMC->PMC_PCER = DEF_BIT(per_id);
}

/*
*********************************************************************************************************
*                                        BSP_PerClkGetStatus()
*
* Description : Get the status of the corresponding peripheral clock.
*
* Argument(s) : per_clk_id    The peripheral clock id to be enabled (see note #1)
*                             BSP_PER_CLK_ID_PIOA
*                             BSP_PER_CLK_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : DEF_TRUE      If the specified clk is enabled 
*               DEF_FALSE     If the specidied clk is disabled.
* 
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/


CPU_BOOLEAN  BSP_PerClkGetStatus  (CPU_INT08U  per_id)
{
    CPU_BOOLEAN status;
   
                                                                  /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return (DEF_FALSE);
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return (DEF_FALSE);
    }
 
    status  = DEF_BIT_IS_SET(AT91C_BASE_PMC->PMC_PCSR, DEF_BIT(per_id));
        
    return (status);
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                   PROGRAMMABLE CLK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_PclkEn()
*
* Description : This function enables the Programmable clock x
*
* Argument(s) : pclk_id     The programmable clock id:
*                           BSP_PCLK_ID0
*                           BSP_PCLK_ID1
*                           BSP_PCLK_ID2
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none
*********************************************************************************************************
*/

void  BSP_PclkEn (CPU_INT08U pclk_id)
{    
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
    if (pclk_id > BSP_PCLK_ID2) { 
        return;
    }
    
    AT91C_BASE_PMC->PMC_SCER =  DEF_BIT(pclk_id);
}

/*
*********************************************************************************************************
*                                            BSP_PclkDis()
*
* Description : This function disables the Programmable clock x
*
* Argument(s) : pclk_id     The programmable clock id:
*                           BSP_PCLK_ID0
*                           BSP_PCLK_ID1
*                           BSP_PCLK_ID2
*
* Return(s)   : The master clock frequency, in Hz.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PclkDis (CPU_INT08U pclk_id)
{
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
    if (pclk_id > BSP_PCLK_ID2) { 
        return;
    }
    
    AT91C_BASE_PMC->PMC_SCDR = DEF_BIT(pclk_id);
}


/*
*********************************************************************************************************
*                                            BSP_PclkGetFreq()
*
* Description : This function gets the Programmable clk x frequency
*
* Argument(s) : pclk_id     The programmable clock id:
*                           BSP_PCLK00_ID
*                           BSP_PCLK01_ID
*                           BSP_PCLK02_ID
*
* Return(s)   : The programable clk frequency in Hz.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_PclkGetFreq (CPU_INT08U  pclk_id)
{
    CPU_INT08U  pclk_css;
    CPU_INT32U  pclk_freq;
    CPU_INT08U  pclk_pre;
              
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
    if (pclk_id > BSP_PCLK_ID2) { 
        return (0);
    }

    pclk_css =  (AT91C_BASE_PMC->PMC_PCKR[pclk_id]        & 0x03);    
    pclk_pre = ((AT91C_BASE_PMC->PMC_PCKR[pclk_id] >> 2)  & 0x07);    
    pclk_pre = DEF_BIT(pclk_pre);
    
    switch (pclk_css) {

        case BSP_CLK_SRC_SLOW: 
             pclk_freq = BSP_SLOW_XTAL_FREQ;
             break;

        case BSP_CLK_SRC_MAIN: 
             pclk_freq = BSP_MainOscGetFreq();
             break;

        case BSP_CLK_SRC_PLL:                           
             pclk_freq = BSP_PLL_GetFreq();
             break;
         
        case BSP_CLK_SRC_RESERVED: 
        default :
             pclk_freq = 0;
    }            

    pclk_freq /= pclk_pre;

    return (pclk_freq);
}

/*
*********************************************************************************************************
*                                            BSP_PclkCfg()
*
* Description : This function configure the programmable clk.
*
* Argument(s) : pclk_id      Programmable clk id:
*                            BSP_PCLK0_ID
*                            BSP_PCLK1_ID
*                            BSP_PCLK2_ID

*
*               pclk_scr     Selects the programmable clk source:
*                            BSP_CLK_SRC_SLOW      
*                            BSP_CK_SRC_MAIN      
*                            BSP_CK_SRC_PLL
*
*               pclk_pre     The processor clk preescaler
*                            BSP_CLK_PRE1   Selected clock
*                            BSP_CLK_PRE2   Selected clock divided by  2
*                            BSP_CLK_PRE4   Selected clock divided by  4
*                            BSP_CLK_PRE8   Selected clock divided by  8
*                            BSP_CLK_PRE16  Selected clock divided by 16
*                            BSP_CLK_PRE32  Selected clock divided by 32
*                            BSP_CLK_PRE64  Selected clock divided by 64
*                                
* Return(s)   : none. 
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  BSP_PclkCfg (CPU_INT08U  pclk_id,                    
                   CPU_INT08U  pclk_src,
                   CPU_INT08U  pclk_pre)
{    
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
    if (pclk_id > BSP_PCLK_ID2) { 
        return;
    }
    
    if ((pclk_src != BSP_CLK_SRC_SLOW) && 
        (pclk_src != BSP_CLK_SRC_MAIN) &&
        (pclk_src != BSP_CLK_SRC_PLL)) {
        return;                
    }
    
    if (pclk_pre > BSP_CLK_PRE64) {
        return;
    }
    
    AT91C_BASE_PMC->PMC_PCKR[pclk_id] = ((pclk_src & 0x03) << 0)
                                      | ((pclk_pre & 0x07) << 2);
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                             PLL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_PLL_GetFreq()
*
* Description : Enable  and configure the PLL
*
* Argument(s) : none.
*
* Return(s)   : DEF_TRUE      If the PLL is enabled and stabilized
*               DEF_FALSE     If the PLL could not be stabilized during a period of time
*                             specified by BSP_PLL_MAX_TIMEOUT
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/
            
CPU_INT32U  BSP_PLL_GetFreq (void)
{
    CPU_INT08U  pll_div;
    CPU_INT16U  pll_mul;
    CPU_INT32U  freq;
  
               
    pll_div = ((AT91C_BASE_PMC->PMC_PLLR >>  0) & 0x0FF);
    pll_mul = ((AT91C_BASE_PMC->PMC_PLLR >> 16) & 0x7FF);

    if (pll_div != 0) {
        freq = (BSP_MAIN_XTAL_FREQ * (pll_mul + 1)) / pll_div;    
    }
    
    return (freq);        
}

/*
*********************************************************************************************************
*                                               BSP_PLL_Set()
*
* Description : Enable  and configure the PLL
*
* Argument(s) : pll_div    The pll divider.            
*
*               pll_mul    The pll Multipler.   
*
*               pll_ctr    The number of slow clock cycles before the LOCKA bit ise set in PMC_SR.
*
*               pll_out    : PLL frequency optimization parameter (see note #1)
*
* Return(s)   : DEF_TRUE   If the PLL is enabled and stabilized
*               DEF_FALSE  If the PLL could not be stabilized during a period of time
*                          specified by BSP_PLL_MAX_TIMEOUT
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/
            
CPU_BOOLEAN  BSP_PLL_Cfg (CPU_INT16U  pll_mul,
                          CPU_INT08U  pll_div,
                          CPU_INT08U  pll_out,
                          CPU_INT08U  pll_ctr)
{
    CPU_INT32U  timeout;
    
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */
    if (pll_mul > BSP_PLL_MAX_MUL) {
        return (DEF_FALSE);
    }
    
    if ((pll_out != 0x00) && 
        (pll_out != 0x01)) {
        return (DEF_FALSE);
    }
    
    AT91C_BASE_PMC->PMC_PLLR  = ((pll_div       ) << 0 )
                              | ((pll_ctr & 0x3F) << 8 )
                              | ((pll_out & 0x03) << 14)
                              | ((pll_mul - 1   ) << 16);
                             
    
    timeout = BSP_PLL_MAX_TIMEOUT;

    while ((timeout > 0) &&     //wait until PLL is locked
          (DEF_BIT_IS_CLR(AT91C_BASE_PMC->PMC_SR, DEF_BIT_02))) {
        timeout--;       
    }
        
    if (timeout == 0) {
        return (DEF_FALSE);
    } else {
        return (DEF_TRUE);  
    }
                 
}

/*
*********************************************************************************************************
*                                          OS_CPU_ExceptHndlr()
*
* Description : Handle any exceptions.
*
* Argument(s) : except_id     ARM exception type:
*
*                                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
*
* Note(s)     : (1) Only OS_CPU_ARM_EXCEPT_FIQ and OS_CPU_ARM_EXCEPT_IRQ exceptions handler are implemented. 
*                   For the rest of the exception a infinite loop is implemented for debuging pruposes. This behavior
*                   should be replaced with another behavior (reboot, etc).
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_INT32U  except_id)
{
    CPU_FNCT_VOID   pfnct;    
    
    switch (except_id) {
        case OS_CPU_ARM_EXCEPT_IRQ:    
             pfnct = (CPU_FNCT_VOID)AT91C_BASE_AIC->AIC_IVR;       /* Read the interrupt vector from the VIC          */
     
             while (pfnct != (CPU_FNCT_VOID)0) {                   /* Make sure we don't have a NULL pointer          */
                 (*pfnct)();                                       /* Execute the ISR for the interrupting device     */
                 AT91C_BASE_AIC->AIC_EOICR = 0;                    /* End of handler                                  */
                 pfnct = (CPU_FNCT_VOID)(AT91C_BASE_AIC->AIC_IVR); /* Read IRQ hanlder from the AIC                   */
             }

             AT91C_BASE_AIC->AIC_EOICR = 0;                        /* End of handler                                  */
             break;

        case OS_CPU_ARM_EXCEPT_FIQ:        
             pfnct = (CPU_FNCT_VOID)AT91C_BASE_AIC->AIC_FVR;       /* Read the interrupt vector from the VIC          */

             while (pfnct != (CPU_FNCT_VOID)0) {                   /* Make sure we don't have a NULL pointer          */

                (*pfnct)();                                        /* Execute the ISR for the interrupting device     */
                AT91C_BASE_AIC->AIC_EOICR = 0;                     /* End of handler                                  */
                pfnct = (CPU_FNCT_VOID)(AT91C_BASE_AIC->AIC_FVR);  /* Read FIQ handler from the AIC                   */
             }

             AT91C_BASE_AIC->AIC_EOICR = 0;                        /* End of handler                                  */
             break;

        case OS_CPU_ARM_EXCEPT_RESET:
             /* $$$$ Insert code to handle a Reset exception               */
        
        case OS_CPU_ARM_EXCEPT_UNDEF_INSTR:
            /* $$$$ Insert code to handle a Undefine Instruction exception */ 

        case OS_CPU_ARM_EXCEPT_SWI:               
            /* $$$$ Insert code to handle a Software exception             */ 
        
        case OS_CPU_ARM_EXCEPT_PREFETCH_ABORT:
            /* $$$$ Insert code to handle a Prefetch Abort exception       */ 
        
        case OS_CPU_ARM_EXCEPT_DATA_ABORT:        
            /* $$$$ Insert code to handle a Data Abort exception           */ 
        
        case OS_CPU_ARM_EXCEPT_ADDR_ABORT:        
            /* $$$$ Insert code to handle a Address Abort exception        */ 
        default:        
            
            while (DEF_TRUE) {    /* Infinite loop on other exceptions. (see note #1)          */
                ;                                               
            }
    }      
}



/*
*********************************************************************************************************
*********************************************************************************************************
*                             uC/Probe PLUG-IN FOR uC/OS-II FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : OSProbe_Init()
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN == 1)
void  OSProbe_TmrInit (void)
{
#if (OS_PROBE_TIMER_SEL == 0)
    BSP_PerClkEn(BSP_PER_ID_TC0);                               /* Enable the peripheral clk                          */
    AT91C_BASE_TCB0->TCB_TC0.TC_CCR  =  DEF_BIT_01;              /* TC0 timer disabled                                 */
    AT91C_BASE_TCB0->TCB_TC0.TC_CMR &= ~(7 <<  0);               /* TIMER_CLOCK1 is input clk                          */
    AT91C_BASE_TCB0->TCB_TC0.TC_CCR  =  DEF_BIT_00;              /* TC0 timer enabled                                  */
    AT91C_BASE_TCB0->TCB_TC0.TC_CCR  =  DEF_BIT_02;              /* SWTRG to reset and start                           */
#endif

#if (OS_PROBE_TIMER_SEL == 1)
    BSP_PerClkEn(BSP_PER_ID_TC1);                               /* Enable the peripheral clk                          */
    AT91C_BASE_TCB0->TCB_TC1.TC_CCR  =  DEF_BIT_01;              /* TC1 timer disabled                                 */
    AT91C_BASE_TCB0->TCB_TC1.TC_CMR &= ~(7 <<  0);               /* TIMER_CLOCK1 is input clk                          */
    AT91C_BASE_TCB0->TCB_TC1.TC_CCR  =  DEF_BIT_00;              /* TC1 timer enabled                                  */
    AT91C_BASE_TCB0->TCB_TC1.TC_CCR  =  DEF_BIT_02;              /* SWTRG to reset and start                           */
#endif

#if (OS_PROBE_TIMER_SEL == 2)
    BSP_PerClkEn(BSP_PER_ID_TC2);                               /* Enable the peripheral clk                          */
    AT91C_BASE_TCB0->TCB_TC2.TC_CCR  =  DEF_BIT_01;              /* TC2 timer disabled                                 */
    AT91C_BASE_TCB0->TCB_TC2.TC_CMR &= ~(7 <<  0);               /* TIMER_CLOCK1 is input clk                          */
    AT91C_BASE_TCB0->TCB_TC2.TC_CCR  =  DEF_BIT_00;              /* TC2 timer enabled                                  */
    AT91C_BASE_TCB0->TCB_TC2.TC_CCR  =  DEF_BIT_02;              /* SWTRG to reset and start                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                        OSProbe_TmrRd()
*
* Description : Read the current counts of a 16-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 16 bit counts (in a 32 bit variable) of the timer.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN == 1)
CPU_INT32U  OSProbe_TmrRd (void)
{
    CPU_INT32U  cnts;


#if (OS_PROBE_TIMER_SEL == 0)
    cnts = (CPU_INT32U)(AT91C_BASE_TCB0->TCB_TC0.TC_CV & 0x0000FFFF);    /* Read timer 0                                     */
#endif

#if (OS_PROBE_TIMER_SEL == 1)
    cnts = (CPU_INT32U)(AT91C_BASE_TCB0->TCB_TC1.TC_CV & 0x0000FFFF);    /* Read timer 1                                     */
#endif

#if (OS_PROBE_TIMER_SEL == 2)
    cnts = (CPU_INT32U)(AT91C_BASE_TCB0->TCB_TC2.TC_CV & 0x0000FFFF);    /* Read timer 2                                     */
#endif

    return (cnts);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-II TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_Tmr_TickInit()
*
* Description : Initialize uC/OS-II's tick source; for the AT91SAM, the Periodic Interval Timer (PIT).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) PIT Interrupt frequency:
*
*                             MCLK        1
*                   Freq =    ---- * -----------
*                              16     (PIV + 1)
*
*
*                             MCLK      1
*                   PIV  =  ( ---- * ------ ) - 1
*                              16     Freq
*
*                   Where:
*
*                       MCLK = Master Clock
*                       Freq = Desired frequency (i.e. OS_TICKS_PER_SEC)
*********************************************************************************************************
*/

static  void  BSP_Tmr_TickInit (void)
{
    CPU_INT32U  cnts;
    CPU_INT32U  mclk_freq;


    mclk_freq = BSP_MclkGetFreq();                              /* Determine the number of counts per tick.           */
    cnts      = ((mclk_freq) / 16 / OS_TICKS_PER_SEC) - 1;

    
    BSP_IntVectSet((CPU_INT08U   )BSP_PER_ID_SYSC,
                   (CPU_INT08U   )0,
                   (CPU_INT08U   )BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE,
                   (CPU_FNCT_VOID)BSP_Sys_ISR_Handler);

    BSP_IntClr(BSP_PER_ID_SYSC);  
    BSP_IntEn(BSP_PER_ID_SYSC);  
                                                                /* Enable the PIT with the correct compare value.      */
    AT91C_BASE_PITC->PITC_PIMR = DEF_BIT_24                     /* Enable the periodic interval timer                  */ 
                               | DEF_BIT_25                     /* Enable the periodic interval timer interrupt        */ 
                               | cnts;                          /* Set the periodic interval value                     */
}


/*
*********************************************************************************************************
*                                       BSP_Tmr_TickISR_Handler()
*
* Description : Handle the PIT interrupt that is used to generate TICKs for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) The general SYS peripheral interrupt handler, Sys_ISR_Handler() issues the initial
*                   interrupt entrance command and clears the SYS interrupt in the AIC.
*********************************************************************************************************
*/

void  BSP_Tmr_TickISR_Handler (void)
{
    CPU_INT32U  status;


    status = AT91C_BASE_PITC->PITC_PIVR;
    (void)status;                                               /* Prevent compiler warning about status set & not used.    */

    OSTimeTick();                                               /* Tell uC/OS-II about clock tick.                          */
}


/*
*********************************************************************************************************
*                                         BSP_Sys_ISR_Handler()
*
* Description : Handle SYS peripheral interrupts.
*
* Argument(s) : none.
*
* Note(s)     : (1) The interrupts from several devices are multiplexed for the SYS interrupt.  This
*                   handler currently handles the DBGU and PIT interrupts.
*
*               (2) AIC interrupts are cleared in this function; each handler called by this function
*                   need only clear its local source.
*********************************************************************************************************
*/
volatile CPU_INT08U  Flag_Reset_Pin_Trigger = 0 ;

void  BSP_Sys_ISR_Handler (void)
{
  
#if ((APP_CFG_PROBE_COM_EN     == DEF_ENABLED) && \
     (PROBE_RS232_CFG_COMM_SEL == PROBE_RS232_UART_DBG))
    CPU_INT32U  dbgu_csr;
    CPU_INT32U  dbgu_imr;
#endif

    AT91C_BASE_AIC->AIC_IVR = 0;          /* Write the IVR, as required in Protection Mode. */
 
    //check if PIT interruption assert
    if ((AT91C_BASE_PITC->PITC_PISR & AT91C_PITC_PITS) ==  AT91C_PITC_PITS) {

        BSP_Tmr_TickISR_Handler();
    }
    
    //check if NRST interruption assert //PQ
    if( AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_URSTS ) {       
        Flag_Reset_Pin_Trigger++ ; 
          
    }   
    
     //check if DBGU interruption assert
#if ((APP_CFG_PROBE_COM_EN     == DEF_ENABLED) && \
     (PROBE_RS232_CFG_COMM_SEL == PROBE_RS232_UART_DBG))
    dbgu_csr = AT91C_BASE_DBGU->DBGU_CSR;
    dbgu_imr = AT91C_BASE_DBGU->DBGU_IMR;

    if ((dbgu_csr & dbgu_imr) != 0) {
        ProbeRS232_RxTxISRHandler();
    }
#endif    
    

    BSP_IntClr(BSP_PER_ID_SYSC);
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                       Serial Port Communications
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                   BSP_Ser_Init()
*
* Description : Initialize a serial port for communication.
*
* Argument(s) : baud_rate: Desire baud rate for serial communication.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_Init (CPU_INT32U baud_rate)
{
    CPU_INT32U  pclk_freq;


    pclk_freq                  = BSP_CPU_ClkFreq();             /* Get the UART input clock frequency                       */

//#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG)  
    if( Debug_COM_Sel == 0 ) {
      
    AT91C_BASE_PIOA->PIO_PDR  |= BSP_GPIOA_DBG;                 /* Set GPIOA pins 9 and 10 as DBGU UART pins                */
    AT91C_BASE_PIOA->PIO_ASR  |= BSP_GPIOA_DBG;                 /* Select GPIOA attached peripheral (DBGU)                  */
    
    AT91C_BASE_PMC->PMC_PCDR   = (1 << AT91C_ID_SYS);
     
    AT91C_BASE_DBGU->DBGU_CR   =  AT91C_US_RSTRX 
                               |  AT91C_US_RSTTX ;             /*  Reset the receiver  and transmitter                      */
      
    AT91C_BASE_DBGU->DBGU_IDR  = AT91C_US_RXRDY                 /* Disable Rx interrupts                                    */
                               | AT91C_US_TXRDY;                /* Disable Tx interrupt                                     */   
    
    AT91C_BASE_DBGU->DBGU_CR   = AT91C_US_RXEN                  /* Enable the receiver                                      */
                               | AT91C_US_TXEN   ;              /* Enable the transmitter                                   */
                      
      
    AT91C_BASE_DBGU->DBGU_MR   = AT91C_US_USMODE_NORMAL         /* Normal mode selected                                     */
                               | AT91C_US_PAR_NONE;             /* No parity bit selected                                   */

                                                                /* Set the DBGU baud rate                                   */
    AT91C_BASE_DBGU->DBGU_BRGR = (CPU_INT16U)((pclk_freq) / baud_rate / 16);

    AT91C_BASE_PMC->PMC_PCER   = (1 << AT91C_ID_SYS);           /* Enable the DBGU peripheral clock                         */
//#endif
    } else {
//#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_UART_01)    
                                                                /* Set GPIOA pins 0 & 1 as US0 pins                         */
    AT91C_BASE_PIOA->PIO_PDR   = BSP_GPIOA_UART0;
    AT91C_BASE_PIOA->PIO_ASR   = BSP_GPIOA_UART0;
    
    AT91C_BASE_US0->US_CR      = AT91C_US_RSTRX 
                               | AT91C_US_RSTTX ;               /* Reset the receiver  and transmitter                      */
    
                                                                /* ---------------------- SETUP US0 ----------------------- */
    AT91C_BASE_US0->US_IDR     = 0xFFFFFFFF;  //disbale all int
    AT91C_BASE_US0->US_CR      = AT91C_US_RXEN  | AT91C_US_TXEN;  /* Enable the receiver  and transmitter                   */                           
                                
    AT91C_BASE_US0->US_MR      = AT91C_US_USMODE_NORMAL         /* RS232C mode selected                                     */
                               | AT91C_US_CLKS_CLOCK            /* USART input CLK is MCK                                   */
                               | AT91C_US_CHRL_8_BITS           /* 8 bit data to be sent                                    */
                               | AT91C_US_PAR_NONE              /* No parity bit selected                                   */
                               | AT91C_US_NBSTOP_1_BIT;         /* 1 stop bit selected                                      */
                                                                /* Set the USART baud rate                                  */
    AT91C_BASE_US0->US_BRGR    = (CPU_INT16U)((pclk_freq) / baud_rate / 16);
  
    AT91C_BASE_US0->US_PTCR    = AT91C_PDC_TXTDIS               //Disable previous PDC settings
                               | AT91C_PDC_RXTDIS;
                                                                /* ---------------- INITIALIZE AIC FOR US0 ---------------- */
    AT91C_BASE_PMC->PMC_PCER   = (1 << AT91C_ID_US0);           /* Enable the US0 peripheral clock                          */
//#endif    
    }
    
}

/*
*********************************************************************************************************
*                                                BSP_Ser_WrByte()
*
* Description : Writes a single byte to a serial port.
*
* Argument(s) : tx_byte     The character to output.
*
* Return(s)   : none.
*
* Note(s)     : (1) This functino blocks until room is available in the UART for the byte to be sent.
*********************************************************************************************************
*/

void BSP_Ser_WrByte(CPU_CHAR tx_byte)
{
  
    unsigned char   err;  
    
    OSSemPend( Bsp_Ser_Tx_Sem_lock, 0, &err );  
    
//#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG) 
    if( Debug_COM_Sel == 0 ) {   
      
        while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY) == 0);  /*  Wait for room in the transmit register.                 */
        AT91C_BASE_DBGU->DBGU_THR = tx_byte;
//#endif    
        
    } else if( Debug_COM_Sel == 1 ){
      
//#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_01) 
        while ((AT91C_BASE_US0->US_CSR & AT91C_US_TXRDY) == 0);  /*  Wait for room in the transmit register.                 */
        AT91C_BASE_US0->US_THR    = tx_byte;
    
//#endif    
    }  else {
    
        // DBG UART used as CHIP_UART connection now
    }
    
    OSSemPost( Bsp_Ser_Tx_Sem_lock );

}

/*
*********************************************************************************************************
*                                                BSP_Ser_WrStr()
*
* Description : Write a character string to a serial port.
*
* Argument(s) : tx_str      A character string.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_WrStr (CPU_CHAR *tx_str)
{
    while ((*tx_str) != 0) {
//        if (*tx_str == '\n') {
//            BSP_Ser_WrByte('\n');
//            BSP_Ser_WrByte('\r');
//            tx_str++;
//        } else {
            BSP_Ser_WrByte(*tx_str++);
//        }        
    }
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdByte()
*
* Description : Read a byte from a serial port and echo byte to port.
*
* Argument(s) : none.
*
* Return(s)   : A byte containing the value of the received charcater.
*
* Note(s)     : (1) This function blocks until a character appears at the port.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Ser_RdByte (void)
{
    CPU_INT08U      rx_byte;
    unsigned char   err; 
    
    OSSemPend( Bsp_Ser_Rx_Sem_lock, 0, &err );  

     if( Debug_COM_Sel == 0 ) {   
       
        //#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG) 
        while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY) == 0) {     /*  Wait for a byte to show up.                         */
            OSTimeDly(2);
        }
        rx_byte = (CPU_INT08U)(AT91C_BASE_DBGU->DBGU_RHR & 0x00FF);     /* Read the character.                                  */
    //#endif  
        
     } else if( Debug_COM_Sel == 1 ){
   
    //#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_01) 
        while ((AT91C_BASE_US0->US_CSR & AT91C_US_RXRDY) == 0) {     /*  Wait for a byte to show up.                         */
            OSTimeDly(2);
        }
        rx_byte = (CPU_INT08U)(AT91C_BASE_US0->US_RHR & 0x00FF);     /* Read the character.                                  */
    //#endif   
        
     } else {
       
        // DBG UART used as CHIP_UART connection now
     }
     
     OSSemPost( Bsp_Ser_Rx_Sem_lock );
    
    return (rx_byte);
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdStr()
*
* Description : Read a string from a serial port.
*
* Argument(s) : rx_str      A pointer to a buffer at which the string can be stored.
*               len         The size of the string that will be read.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_RdStr (CPU_CHAR    *rx_str,
                     CPU_INT32U   len)
{
    CPU_CHAR  input;
    CPU_CHAR  input_ix;


    input_ix  = 0;
    rx_str[0] = 0;

    while (DEF_TRUE)
    {
        input = BSP_Ser_RdByte();

        if ((input == '\r') ||
            (input == '\n')) {
            BSP_Ser_Printf("\n");
            rx_str[input_ix] = 0;
            break;
        }

        if (input == '\b') {
            if (input_ix > 0) {
                BSP_Ser_Printf("\b \b");
                input_ix--;
                rx_str[input_ix] = 0;
            }
        }

        if (ASCII_IS_PRINT(input)) {
            BSP_Ser_Printf("%c", input);
            rx_str[input_ix] = input;
            input_ix++;
            if (input_ix >= len) {
               input_ix = len;
            }
        }
    }
}

/*
*********************************************************************************************************
*                                                BSP_Ser_Printf()
*
* Description : Formatted outout to the serial port.
*
* Argument(s) : format      Format string follwing the C format convention.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_Printf (CPU_CHAR *format, ...)
{
    static  CPU_CHAR  buffer[80 + 1];
            va_list   vArgs;


    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);

    BSP_Ser_WrStr((CPU_CHAR*) buffer);
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         USB CLK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            BSP_USBclkEn()
*
* Description : This functions enables the 48 Mhz clock of the USB Device port 
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.

* Note(s)     : None
*********************************************************************************************************
*/

void  BSP_USBclkEn (void) 
{
    AT91C_BASE_PMC->PMC_SCER = DEF_BIT_07;
}

/*
*********************************************************************************************************
*                                            BSP_USBclkDis()
*
* Description : This functions disables the 48 Mhz clock of the USB Device port 
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.

* Note(s)     : None
*********************************************************************************************************
*/

void  BSP_USBclkDis (void) 
{
    AT91C_BASE_PMC->PMC_SCDR = DEF_BIT_07;
}

/*
*********************************************************************************************************
*                                         BSP_USBclkGetFreq()
*
* Description : Get the USB clk frequency
*
* Argument(s) : none.
*
* Return(s)   : The USB clk frequency in Hz.
*
* Caller(s)   : Application.

* Note(s)     : None
*********************************************************************************************************
*/

CPU_INT32U  BSP_USBclkGetFreq (void) 
{
    CPU_INT32U  pll_freq;
    CPU_INT08U  div;    


    pll_freq  = BSP_PLL_GetFreq();             
    div       = (AT91C_BASE_PMC->PMC_PLLR >> 8);
    div       = DEF_BIT(div);

    pll_freq /= div;
    
    return (pll_freq);
}

/*
*********************************************************************************************************
*                                         BSP_USBclkCfg()
*
* Description : Configure the USB Clk
*
* Argument(s) : div       The USB clk divider
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_USBclkCfg (CPU_INT08U div) 
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (div > BSP_USB_CLK_DIV4) {
        return;    
    }
    
    DEF_BIT_CLR(AT91C_BASE_PMC->PMC_PLLR, (DEF_BIT_28 | DEF_BIT_29));
    DEF_BIT_SET(AT91C_BASE_PMC->PMC_PLLR, (div << 28));

}


/*
*********************************************************************************************************
*                                         Beep()
*
* Description : Beep Buzzer
*
* Argument(s) : beep times.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskJoy()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void Beep( INT32U beep_cycles)
{
  
   INT32U i ;
   
   for(i = 0; i< beep_cycles; i++)  {
    
        PIO_Clear(&PinBuzzer); //beep on
        LED_Clear(LED_DS1); 
        LED_Set(LED_DS2);
        OSTimeDly(250);           
        PIO_Set(&PinBuzzer); //beep off
        LED_Clear(LED_DS2); 
        LED_Set(LED_DS1);
        OSTimeDly(250); //delay_ms(250);  
    
    }
   
}


/*
*********************************************************************************************************
*                                         get_os_state()
*
* Description : get_os_state
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

const CPU_INT08S os_stat_desp[][6] = {"Sem  ","MBox ","Q    ","Suspd","Mutex","Flag ","  *  ","Multi","Ready"} ;

CPU_INT08S* get_os_state( INT8U os_state ) 
{
  
  CPU_INT08U i = 0 ;

  for(i=0;i<8;i++) {
    if(os_state == (1<<i) ) {     
      return  (CPU_INT08S*)os_stat_desp[i];      
    }
  }
  
  return  (CPU_INT08S*)os_stat_desp[i];  
  
}


/*
*********************************************************************************************************
*                                         Get_Task_Info()
*
* Description : Print task related information
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  Get_Task_Info (void)
{
    OS_TCB    *ptcb;
    CPU_INT08U index = 1 ;
    
    if (OSRunning == OS_TRUE) {
     
        APP_TRACE_INFO(("------------------------------------------------------   DEBUG INFORMATION   -------------------------------------------------------\r\n"));
        APP_TRACE_INFO(("|-------------------------   T  A  S  K   --------------------------------------|----------------   S  T  A  C  K   ---------------|\r\n"));
        APP_TRACE_INFO(("| ID  |    Name    | Priority | CtxSwCtr | State | Delay |      Waitting On     |  Point@ | Cur. | Max. | Size | Starts@ | Ends@   |\r\n"));
        ptcb = OSTCBList;                                  /* Point at first TCB in TCB list               */
        while (ptcb->OSTCBPrio != OS_TASK_IDLE_PRIO) {     /* Go through all TCBs in TCB list              */          
            //APP_TRACE_INFO(( "|%4d ",ptcb->OSTCBId ));  //same as ptcb->OSTCBPrio
            APP_TRACE_INFO(( "|%2d ",index++ ));
            APP_TRACE_INFO(( "%13.13s",ptcb->OSTCBTaskName ));
            APP_TRACE_INFO(( "      %2d ",ptcb->OSTCBPrio ));
            APP_TRACE_INFO(( "  %10d ",ptcb->OSTCBCtxSwCtr ));            
            APP_TRACE_INFO(( "   %s  ",get_os_state( ptcb->OSTCBStat )  ));
            APP_TRACE_INFO(( " %5d ",ptcb->OSTCBDly ));
            APP_TRACE_INFO(( " %22.22s ", (INT32U)(ptcb->OSTCBEventPtr) == 0 ?  (INT8U *)" " : ptcb->OSTCBEventPtr->OSEventName ));  
              
            APP_TRACE_INFO(( " %08X ",ptcb->OSTCBStkPtr ));
            APP_TRACE_INFO(( " %4d ",(ptcb->OSTCBStkBase - ptcb->OSTCBStkPtr)*4 ));
            APP_TRACE_INFO(( " %5d ",ptcb->OSTCBStkUsed ));
            APP_TRACE_INFO(( " %5d ",ptcb->OSTCBStkSize * 4 ));
            APP_TRACE_INFO(( " %08X ",ptcb->OSTCBStkBase ));
            APP_TRACE_INFO(( " %08X ",ptcb->OSTCBStkBottom ));           
            APP_TRACE_INFO(( " |\r\n" ));     
            ptcb = ptcb->OSTCBNext;                        /* Point at next TCB in TCB list                */          
        }
        APP_TRACE_INFO(("------------------------------------------------------------------------------------------------------------------------------------\r\n"));  
         
    }
}

/*
*********************************************************************************************************
*                                         Get_Run_Time()
*
* Description : Print run time information
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void Get_Run_Time( void )
{
  
    INT32U time   ;
    INT8U  sec ;
    INT8U  min ;
    INT8U  hour ;
    INT8U  day ;
  
    time = OSTime / 1000L ;
    sec  = time % 60 ;
    min  = time / 60 %60 ;
    hour = time / 3600 % 24 ;
    day  = time / 3600 / 24 ;
    
    APP_TRACE_INFO(("OS Running Time  =  %02d days : %02d hours : %02d min : %02d sec\r\n", day,hour,min, sec )); 
   
    
}


/*
*********************************************************************************************************
*                                         Head_Info()
*
* Description : Print Head information
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskUserIF()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void Head_Info ( void )
{ 
    FLASH_INFO flash_info;
    Read_Flash_State(&flash_info);
    APP_TRACE_INFO(("\r\n\r\n")); 
    APP_TRACE_INFO(("-----------------------------------------------------------\r\n"));
    APP_TRACE_INFO(("----                    Fortemedia                    -----\r\n"));
    APP_TRACE_INFO(("----             EVM Unified Interface Board          -----\r\n"));
    APP_TRACE_INFO(("----   %20s-%s             -----\r\n", hw_model, fw_version));
    APP_TRACE_INFO(("----   Compile date:  %12s, %8s, by PQ   -----\r\n", __DATE__, __TIME__));
    APP_TRACE_INFO(("-----------------------------------------------------------\r\n"));
    APP_TRACE_INFO(("\r\n"));                
    //APP_TRACE_INFO(("------------------------------------------------------------------------------------------------------------------------------------\r\n"));  
    APP_TRACE_INFO(("Micrium uC/OS-II on the Atmel AT91SAM7A3. Version : V%d.%d \r\n",(OSVersion()/ 100),(OSVersion() % 100)  ));
    APP_TRACE_INFO(("CPU Usage = %d%%, CPU Speed = %3d MHz, Tick_Per_Second = %6d ticks/sec  \r\n", OSCPUUsage,  (BSP_CPU_ClkFreq() / 1000000L),OS_TICKS_PER_SEC ));
    APP_TRACE_INFO(("#Ticks = %8d, #CtxSw = %8d \r\n", OSTime, OSCtxSwCtr )); 
    Get_Run_Time();
    APP_TRACE_INFO(("\r\n"));  
    APP_TRACE_INFO(("-------------------------------------------------   GLOBAL VARIABLES STATUS   ------------------------------------------------------\r\n")); 
    APP_TRACE_INFO(("MEM_Part_MsgUART :         %2d / %2d   of the memory partiation used\r\n", pMEM_Part_MsgUART->OSMemNBlks - pMEM_Part_MsgUART->OSMemNFree,  pMEM_Part_MsgUART->OSMemNBlks)); 
    APP_TRACE_INFO(("Tx_ReSend_Happens:         %7d   times happened\r\n", Tx_ReSend_Happens ));
    APP_TRACE_INFO(("Tx_ReSend_Happens_Ruler:   %7d   times happened\r\n", Tx_ReSend_Happens_Ruler ));
    APP_TRACE_INFO(("TWI_Sem_lock:              %7d   ( default 1 )\r\n", TWI_Sem_lock->OSEventCnt ));   
    //APP_TRACE_INFO(("TWI_Sem_done:              %7d   ( default 0 )\r\n", TWI_Sem_done->OSEventCnt ));
    APP_TRACE_INFO(("UART_MUX_Sem_lock:         %7d   ( default 1 )\r\n", UART_MUX_Sem_lock->OSEventCnt ));
    APP_TRACE_INFO(("Done_Sem_RulerUART:        %7d   ( default 0 )\r\n", Done_Sem_RulerUART->OSEventCnt ));
    APP_TRACE_INFO(("Global_Ruler_State[3..0]:        [%d - %d - %d - %d]\r\n", Global_Ruler_State[3],Global_Ruler_State[2],Global_Ruler_State[1],Global_Ruler_State[0] ));
    APP_TRACE_INFO(("Global_Ruler_Type[3..0] :        [%X - %X - %X - %X]\r\n", Global_Ruler_Type[3],Global_Ruler_Type[2],Global_Ruler_Type[1],Global_Ruler_Type[0] ));
    APP_TRACE_INFO(("Global_Mic_Mask[3..0][] :        [%X - %X - %X - %X]\r\n", Global_Mic_Mask[3],Global_Mic_Mask[2],Global_Mic_Mask[1],Global_Mic_Mask[0] ));
    APP_TRACE_INFO(("Flash Write Cycle:       State_Page = %d cycles,  FW_Bin_Page = %d cycles\r\n", flash_info.s_w_counter,flash_info.f_w_counter ));
    APP_TRACE_INFO(("Ruler FW Bin File:       \"%s\" (%d Bytes), [0x%0X, %s]\r\n", (flash_info.f_w_state == FW_DOWNLAD_STATE_FINISHED ? flash_info.bin_name : " -- "), flash_info.bin_size, flash_info.f_w_state,(flash_info.f_w_state == FW_DOWNLAD_STATE_FINISHED ? "OK" : "Error")));
    APP_TRACE_INFO(("Test Counter:            test_counter1, 2, 3, 4  =  %4d,%4d,%4d,%4d\r\n",  test_counter1, test_counter2,test_counter3, test_counter4));
    APP_TRACE_INFO(("Test Counter:  UART_WriteStart Failed :  %4d  times\r\n",   test_counter5));
    APP_TRACE_INFO(("\r\n"));
    Get_Task_Info ();
    //APP_TRACE_INFO(("\r\n")); 

}



/*
*********************************************************************************************************
*                                         PDM_Pattern_Gen()
*
* Description : Debug_Send_PDM_Pattern_ for CP2240
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskUserIF()
*
* Note(s)     : none.
*********************************************************************************************************
*/
 
//#define PDM_DAT         0   //PA26
//#define PDM_CLK         1   //PA27
//#define PATTERN_DATA    0xD4
//#define IDLE_DATA       0xCC
//#define PATTERN_WIDTH   1 // 1bytes
//#define REPEAT_TIMES    150
//
//
//
//void __ramfunc PDM_Out( unsigned int data, unsigned int times )
//{
//   
//    unsigned int i, j ; 
//    
//    Pin PinsPDM[]   = {GPIO_PA26, GPIO_PA27}; //faster
//    
//    PinsPDM[PDM_DAT].pio->PIO_OER = PinsPDM[PDM_DAT].mask; //set pin as output
//    PinsPDM[PDM_CLK].pio->PIO_OER = PinsPDM[PDM_CLK].mask;
//    PinsPDM[PDM_DAT].pio->PIO_PPUER = PinsPDM[PDM_DAT].mask; //set pull up 
//    PinsPDM[PDM_CLK].pio->PIO_PPUER = PinsPDM[PDM_CLK].mask;
//
//    
//    PinsPDM[PDM_DAT].pio->PIO_CODR = PinsPDM[PDM_DAT].mask;
//    PinsPDM[PDM_CLK].pio->PIO_CODR = PinsPDM[PDM_CLK].mask;
//    
//    for( i = 0; i < times; i++ ) {
//    
//        for( j = 0; j < (8*PATTERN_WIDTH) ; j++)  {
//              
//              PinsPDM[PDM_CLK].pio->PIO_CODR = PinsPDM[PDM_CLK].mask;
//              if( (data<<j)&0x80 ) {              
//                 PinsPDM[PDM_DAT].pio->PIO_SODR = PinsPDM[PDM_DAT].mask;
//              } else {                 
//                 PinsPDM[PDM_DAT].pio->PIO_CODR = PinsPDM[PDM_DAT].mask;
//              }      
//              PinsPDM[PDM_CLK].pio->PIO_SODR = PinsPDM[PDM_CLK].mask;
//          
//               __asm ("NOP");
//               __asm ("NOP");
//               __asm ("NOP");
//               __asm ("NOP");
//               __asm ("NOP");
//               __asm ("NOP");
//             
//        }
//  
//    }
//    
//   PinsPDM[PDM_DAT].pio->PIO_ODR = PinsPDM[PDM_DAT].mask; //set pin  as input
//   PinsPDM[PDM_CLK].pio->PIO_ODR = PinsPDM[PDM_CLK].mask;
//   PinsPDM[PDM_DAT].pio->PIO_PPUDR = PinsPDM[PDM_DAT].mask; //disable pull up 
//   PinsPDM[PDM_CLK].pio->PIO_PPUDR = PinsPDM[PDM_CLK].mask;
//   
//}
//
//
//void   PDM_Pattern_Gen( INT8U type )
//{
//       
//#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
//    OS_CPU_SR  cpu_sr = 0u;
//#endif
//    
//  
//    OS_ENTER_CRITICAL();
//    
//    if( type == 0 ) {
//        PDM_Out(PATTERN_DATA, REPEAT_TIMES);
//    } else {
//        PDM_Out(IDLE_DATA, 0xFFFFFFFF );
//    }
//  
//    OS_EXIT_CRITICAL();
//  
//}