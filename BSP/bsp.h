/*
*********************************************************************************************************
*                               iSAM TEST BENCH AUDIO BRIDGE BOARD APP PACKAGE
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
* Filename      : bsp.h
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
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  BSP_MODULE_PRESENT
#define  BSP_MODULE_PRESENT

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <cpu.h>
#include  <at91sam7a3.h>
#include  <app_cfg.h>
#include  <ucos_ii.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <string.h>
#include  <stdbool.h>
#include  <probe_com_cfg.h>

#include  <taskcomm.h>
#include  <kfifo.h>
#include  <board.h>
#include  <uart.h>
#include  <pio.h>
#include  <pio_it.h>
#include  <gpio.h>
#include  <led.h>
#include  <timer.h>
#include  <efc.h>
#include  <spi.h>
#include  <flashd.h>
#include  <twid.h>
#include  <i2c_gpio.h>
#include  <ruler.h>
#include  <emb.h>
#include  <mem_basic.h>
#include  <uif.h>
#include  <dsp.h>
#include  <noah_cmd.h>
#include  <commands.h>
#include  <codec.h>
#include  <xmodem.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
                                  Board type defines 
*********************************************************************************************************
*/

//#define  BOARD_TYPE_AB01  
//#define  BOARD_TYPE_AB02  
//#define  BOARD_TYPE_AB03  
#define  BOARD_TYPE_UIF


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  BSP_MAIN_XTAL_FREQ                   18432000          /* Onboard Main crystal Osc. frequency (hz)           */
#define  BSP_SLOW_XTAL_FREQ                      32768          /* onboard Slow crystal Osc. frequency (hz)           */


                                                                /* ------------------- CLK SOURCES ------------------ */
#define  BSP_CLK_SRC_SLOW                            0          /* Slow  clock                                        */
#define  BSP_CLK_SRC_MAIN                            1          /* Main  clock                                        */
#define  BSP_CLK_SRC_RESERVED                        2          /* Reserved value                                     */
#define  BSP_CLK_SRC_PLL                             3          /* PLL   clock                                        */

                                                                /* -------------- CLOCK PREESCALERS  ---------------- */
#define  BSP_CLK_PRE1                                0          /* Selected clock                                     */
#define  BSP_CLK_PRE2                                1          /* Selected clock divided by  2                       */
#define  BSP_CLK_PRE4                                2          /* Selected clock divided by  4                       */
#define  BSP_CLK_PRE8                                3          /* Selected clock divided by  8                       */
#define  BSP_CLK_PRE16                               4          /* Selected clock divided by 16                       */
#define  BSP_CLK_PRE32                               5          /* Selected clock divided by 32                       */
#define  BSP_CLK_PRE64                               6          /* Selected clock divided by 64                       */

                                                                /* ------------------- USB DIVIDERS  ---------------- */
#define  BSP_USB_CLK_DIV1                            0          /* Divider output is PLL clock output                 */
#define  BSP_USB_CLK_DIV2                            1          /* Divider output is PLL clock output divided by 2    */
#define  BSP_USB_CLK_DIV4                            2          /* Divider output is PLL clock output divided by 2    */


                                                                /* ----------------- PERIPHRALS ID's ---------------- */
                                                                /* --------- Peripheral Name -------- | -- Ext Int -- */
#define  BSP_PER_ID_AIC_FIQ                          0          /* Advance Interrupt Controller       | FIQ           */
#define  BSP_PER_ID_SYSC                             1          /* System Controller Interrupt        |               */
#define  BSP_PER_ID_PIOA                             2          /* Parallel I/O Controller A          |               */
#define  BSP_PER_ID_PIOB                             3          /* Parallel I/O Controller B          |               */
#define  BSP_PER_ID_CAN0                             4          /* CAN Controller                     |               */  
#define  BSP_PER_ID_CAN1                             5          /* CAN Controller                     |               */
#define  BSP_PER_ID_US0                              6          /* USART 0                            |               */
#define  BSP_PER_ID_US1                              7          /* USART 1                            |               */
#define  BSP_PER_ID_US2                              8          /* USART 1                            |               */
#define  BSP_PER_ID_TWI                             10          /* Two Wire Interface                 |               */
#define  BSP_PER_ID_SPI0                            11          /* Serial Peripheral Interface 1      |               */
#define  BSP_PER_ID_SPI1                            12          /* Serial Peripheral Interface 1      |               */
#define  BSP_PER_ID_SSC0                            13          /* Synchronous Serial Controller      |               */
#define  BSP_PER_ID_SSC1                            14          /* Synchronous Serial Controller      |               */
#define  BSP_PER_ID_TC0                             15          /* Timer/Counter 0                    |               */
#define  BSP_PER_ID_TC1                             16          /* Timer/Counter 1                    |               */
#define  BSP_PER_ID_TC2                             17          /* Timer/counter 2                    |               */
#define  BSP_PER_ID_TC3                             18          /* Timer/Counter 3                    |               */
#define  BSP_PER_ID_TC4                             19          /* Timer/Counter 4                    |               */
#define  BSP_PER_ID_TC5                             20          /* Timer/counter 5                    |               */
#define  BSP_PER_ID_TC6                             21          /* Timer/Counter 0                    |               */
#define  BSP_PER_ID_TC7                             22          /* Timer/Counter 1                    |               */
#define  BSP_PER_ID_TC8                             23          /* Timer/counter 2                    |               */
#define  BSP_PER_ID_ADC0                            24          /* Analog to Digital Converter        |               */
#define  BSP_PER_ID_ADC1                            25          /* Analog to Digital Converter        |               */
#define  BSP_PER_ID_UDP                             27          /* USB Device Port                    |               */
#define  BSP_PER_ID_AIC_IRQ0                        28          /* Advance Interrupt Controller       | IRQ0          */
#define  BSP_PER_ID_AIC_IRQ1                        29          /* Advance Interrupt Controller       | IRQ1          */
#define  BSP_PER_ID_AIC_IRQ2                        30          /* Advance Interrupt Controller       | IRQ2          */
#define  BSP_PER_ID_AIC_IRQ3                        31          /* Advance Interrupt Controller       | IRQ3          */

                                                                /* ------------ INTERRUPT SOURCE TYPE --------------- */
#define  BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE   0          /* Internal Interrupt High Level Sensitive            */
#define  BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG     1          /* Internal Interrupt Positive Edge Trigerred         */
#define  BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE    2          /* External Interrupt Low Level Sensitive             */
#define  BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE   3          /* External Interrupt Negative Edge Trigerred         */
#define  BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG     4          /* External Interrupt High Level Sensitive            */
#define  BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG     5          /* External Interrupt Positive Edge triggered         */

                                                                /* -------- PROGRAMMABLE CLOCK OUTPUT IDs ----------- */
#define  BSP_PCLK_ID0                                0          /* Programmable clock 1                               */
#define  BSP_PCLK_ID1                                1          /* Programmable clock 2                               */
#define  BSP_PCLK_ID2                                2          /* Programmable clock 3                               */

                                                                /* ------------------- UART PORTS ------------------- */
#define  BSP_SER_COMM_UART_01                        2          /* UART 1.   Label as "RS232 COM PORT"                */
#define  BSP_SER_COMM_UART_DBG                      63          /* UART DBG. Label as "SERIAL DEBUG PORT"             */


/*
*********************************************************************************************************
*                                        BACKWARDS COMPATIBILITY
*********************************************************************************************************
*/

#define  Ser_Init           BSP_Ser_Init
#define  Ser_WrByte         BSP_Ser_WrByte
#define  Ser_WrStr          BSP_Ser_WrStr
#define  Ser_RdByte         BSP_Ser_RdByte
#define  Ser_RdStr          BSP_Ser_RdStr
#define  Ser_Printf         BSP_Ser_Printf

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          GENERAL BSP FUNCTIONS
*********************************************************************************************************
*/

void         BSP_Init                (void);
CPU_INT32U   BSP_CPU_ClkFreq         (void);

/*
*********************************************************************************************************
*                                INTERRUPT VECTOR CONTROLLER FUNCTIONS
*********************************************************************************************************
*/
void         BSP_IntClr              (CPU_INT08U     per_id);
void         BSP_IntDis              (CPU_INT08U     per_id);
void         BSP_IntDisAll           (void);
void         BSP_CPU_Init            (void);
void         BSP_IntEn               (CPU_INT08U     per_id);
void         BSP_IntVectSet          (CPU_INT08U     per_id,
                                      CPU_INT08U     int_prio,
                                      CPU_INT08U     int_src_type,
                                      CPU_FNCT_VOID  int_isr_fnct);


/*
*********************************************************************************************************
*                                           JOYSTICK SERVICES
*********************************************************************************************************
*/

CPU_INT08U   BSP_JoyGetPos           (void);
CPU_BOOLEAN  BSP_JoyGetStatus        (CPU_INT08U     joy_pos);

/*
*********************************************************************************************************
*                                             MAIN OSCILLATOR
*********************************************************************************************************
*/

void         BSP_MainOscDis          (void);
CPU_BOOLEAN  BSP_MainOscEn           (CPU_INT08U    start_time);
CPU_INT32U   BSP_MainOscGetFreq      (void);

/*
*********************************************************************************************************
*                                               MASTER CLK
*********************************************************************************************************
*/

CPU_INT32U   BSP_MclkGetFreq         (void);
CPU_BOOLEAN  BSP_MclkCfg             (CPU_INT08U     mclk_src,
                                      CPU_INT08U     cpu_clk_pre);

/*
*********************************************************************************************************
*                                         PUSH BUTTON SERVICES
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PB_GetStatus        (CPU_INT08U     pb);

/*
*********************************************************************************************************
*                                         PROGRAMMABLE CLK
*********************************************************************************************************
*/

CPU_INT32U   BSP_PclkGetFreq         (CPU_INT08U     pclk_id);
void         BSP_PclkCfg             (CPU_INT08U     pclk_id,
                                      CPU_INT08U     pclk_src,
                                      CPU_INT08U     pclk_pre);
/*
*********************************************************************************************************
*                                         PERIPHERAL FUNCTIONS
*********************************************************************************************************
*/

void         BSP_PerClkDis           (CPU_INT08U     per_id);
void         BSP_PerClkEn            (CPU_INT08U     per_id);
CPU_BOOLEAN  BSP_PerClkGetStatus     (CPU_INT08U     per_id);

/*
*********************************************************************************************************
*                                            PLL FUNCTIONS
*********************************************************************************************************
*/

CPU_INT32U   BSP_PLL_GetFreq         (void);
CPU_BOOLEAN  BSP_PLL_Cfg             (CPU_INT16U  pll_mul,
                                      CPU_INT08U  pll_div,
                                      CPU_INT08U  pll_out,
                                      CPU_INT08U  pll_ctr);

/*
*********************************************************************************************************
*                                   EXTERNAL MEMORY INITIALIZATION
*********************************************************************************************************
*/

void         BSP_SDRAM_Init          (void);

/*
*********************************************************************************************************
*                                             RS-232 SERVICES
*********************************************************************************************************
*/

void         BSP_Ser_Init            (CPU_INT32U     baud_rate);
void         BPS_Ser_WrByte          (CPU_CHAR       tx_byte);
void         BSP_Ser_WrStr           (CPU_CHAR      *tx_str);
CPU_INT08U   BSP_Ser_RdByte          (void);
void         BSP_Ser_RdStr           (CPU_CHAR      *rx_str,
                                      CPU_INT32U     len);
void         BSP_Ser_Printf          (CPU_CHAR      *format, ...);


/*
*********************************************************************************************************
*                                           USB CLOCK FUNCTIONS
*********************************************************************************************************
*/
void        BSP_USBclkDis            (void);
void        BSP_USBclkEn             (void);
CPU_INT32U  BSP_USBclkGetFreq        (void);
void        BSP_USBclkCfg            (CPU_INT08U     div);

/*
*********************************************************************************************************
*                                             uC/OS-II SERVICES
*********************************************************************************************************
*/

void        BSP_Tmr_TickISR_Handler  (void);
void        BSP_Sys_ISR_Handler      (void);

/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if    !defined(BSP_CFG_SER_COMM_SEL)
#error  "BSP_CFG_SER_COMM_SEL   not  #define'd in 'app_cfg.h'              "
#error  "                                 [MUST be  BSP_SER_COMM_UART_01]  "
#error  "                                 [     ||  BSP_SER_COMM_UART_DBG] "

#elif   (BSP_CFG_SER_COMM_SEL != BSP_SER_COMM_UART_00 ) && \
        (BSP_CFG_SER_COMM_SEL != BSP_SER_COMM_UART_DBG)
#error  "BSP_CFG_SER_COMM_SEL  illegally #define'd in 'app_cfg.h'           "
#error  "                                 [MUST be  BSP_SER_COMM_UART_00]  "
#error  "                                 [     ||  BSP_SER_COMM_UART_DBG] "
#endif

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

extern CPU_INT08U Debug_COM_Sel;
extern OS_EVENT *Bsp_Ser_Tx_Sem_lock;  
extern OS_EVENT *Bsp_Ser_Rx_Sem_lock;

extern const CPU_CHAR fw_version[];
extern const CPU_CHAR hw_version[];
extern const CPU_CHAR hw_model[];

extern void Head_Info( void );
extern void Beep( INT32U beep_cycles);
extern volatile CPU_INT08U  Flag_Reset_Pin_Trigger;
//extern void PDM_Pattern_Gen( INT8U type );
extern void Time_Stamp( void );
extern void  Get_Flash_Info (void);


#endif                                                          /* End of bsp module include.                         */