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
*                                           TASK PACKAGE
*
*                                          Atmel AT91SAM7A3
*                                               on the
*                                      Unified EVM Interface Board
*
* Filename      : task_uart_rx.c
* Version       : V1.0.0
* Programmer(s) : PQ
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


#include <includes.h>



volatile CPU_INT08U  Global_Idle_Ready = 0 ; //flag check if no command from PC for sometime

/*
*********************************************************************************************************
*                                    App_TaskUART_Rx()
*
* Description : Process UART Receive related process between Audio Bridge and PC, Audio Bridge and Ruler.
*               Fetch data from PC in UART receive buffer, check data sanity in DL layer 
*
* Argument(s) : p_arg   Argument passed to 'App_TaskUART_Rx()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
void App_TaskUART_Rx( void *p_arg )
{     
   (void)p_arg; 
    
    CPU_INT16U       temp ;	
    CPU_INT16U       counter ;	
    CPU_INT08U       idle_counter ;	    
    CMDREAD          CMD_Read_PC ;
    CMDREAD          CMD_Read_Ruler ;
    
    CPU_INT08U       rx_data[256];
        
    Init_CMD_Read( &CMD_Read_PC, EVENT_MsgQ_PCUART2Noah ) ;
    Init_CMD_Read( &CMD_Read_Ruler, EVENT_MsgQ_RulerUART2Noah ) ;
    idle_counter = 0;
    
	while (DEF_TRUE) {               
   
        //counter = Queue_NData( (void*) pUART_Rece_Buf[PC_UART] ) ;  
        counter  = kfifo_get_data_size(pUART_Rece_kfifo[PC_UART]); 
        //APP_TRACE_DBG((" %4d ",counter)) ;  
        if( counter ) {
            idle_counter = 0 ;
        } else {            
            if( idle_counter++ >= 100 ) { // 100*5ms = 500ms
                Global_Idle_Ready = 1 ;                
                idle_counter = 0 ;
                LED_Clear(LED_DS2); //mute communication LED when >500ms free
            }   
        }
        
        if( counter ) {
//             APP_TRACE_INFO(("\r\n\r\n---"));
//             Time_Stamp();
//             APP_TRACE_INFO(("\r\n:App_TaskUART_Rx check: [%d]start",counter));        
//           
             //Queue_ReadBuf( rx_data, pUART_Rece_Buf[PC_UART], counter > 256 ? 256 : counter , &temp );
             counter = counter < 256 ? counter : 256;
             kfifo_get(pUART_Rece_kfifo[PC_UART], (unsigned char *)rx_data, counter) ; 
             for(unsigned int i = 0; i< counter; i++ ){                 
                 Noah_CMD_Read( &CMD_Read_PC, rx_data[i] ) ;  
                 
             }
   
//          while( counter-- ) {              
//            Queue_Read( &temp, pUART_Rece_Buf[PC_UART] );            
//            Noah_CMD_Read( &CMD_Read_PC, temp ) ;
//            
//		}  
          
//             Time_Stamp();
//             APP_TRACE_INFO(("\r\n:App_TaskUART_Rx check: end "));
//         
         
             
        }

        
//        counter = Queue_NData( (void*) pUART_Rece_Buf[RULER_UART] ) ;         
//        while( counter-- ) {              
//            Queue_Read( &temp, pUART_Rece_Buf[RULER_UART] );            
//            Noah_CMD_Read( &CMD_Read_Ruler, temp ) ;
//        } 
        
        OSTimeDly(5); // note : UART1_RECE_QUEUE_LENGTH = 1024B; 115200/10/1000 =  11.52;
  
                    
	}
    
}


    