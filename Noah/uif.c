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
*                                        FM DSP W/R RELATED OPERATIONS
*
*                                          Atmel AT91SAM7A3
*                                               on the
*                                      Unified EVM Interface Board
*
* Filename      : uif.c
* Version       : V1.0.0
* Programmer(s) : PQ
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


#include <includes.h>


INTERFACE_CFG   Global_UIF_Setting[3];     //ruler type
unsigned char   Reg_RW_Data[4];


extern EMB_BUF   Emb_Buf_Data;
extern EMB_BUF   Emb_Buf_Cmd;


//disabled endian reverse, as PC scripts will do it!
void Reverse_Endian( unsigned char *pdata, unsigned char size ) 
{
    
//    unsigned char i;
//    unsigned char temp;
//    
//    for( i = 0 ; i< size>>1 ; i++ ) {
//      temp = *( pdata+i );
//      *( pdata+i ) = *( pdata + size -1 - i );
//      *( pdata + size -1 - i ) = temp;
//      
//    }      
    
}



/*
*********************************************************************************************************
*                                           Setup_Interface()
*
* Description : Send command to start USB audio play/record.
* Argument(s) : cmd_type : record£¨== 1£©/play£¨== 2£©/record & play £¨== 3£©
*               padding :  used for usb audio BI/BO first package padding
* Return(s)   : NO_ERR :   execute successfully
*               others :   refer to error code defines.           
*
* Note(s)     : None.
*********************************************************************************************************
*/
unsigned char Setup_Interface( INTERFACE_CFG *pInterface_Cfg )
{  
    
    unsigned char err; 
    unsigned int  temp;
    
    APP_TRACE_INFO(("\r\nSetup_Interface: if_type=%d, speed=%dkHz, attribute=0x%X\r\n",\
                         pInterface_Cfg->if_type,pInterface_Cfg->speed, pInterface_Cfg->attribute));
    
    err = NULL;
    temp  = pInterface_Cfg->speed ;
    
    switch( pInterface_Cfg->if_type )  {
        
        case UIF_TYPE_I2C :
            if( temp <= 400 && temp >= 10) { 
                TWI_Init( temp * 1000 );     
                APP_TRACE_INFO(("\r\nI2C port is re-initialized to %d kHz\r\n",temp));        
            }  else {
                APP_TRACE_INFO(("\r\nERROR: I2C speed not support %d kHz\r\n",temp));
                err = SET_I2C_ERR ;
            }   
        break ;  
        
        case UIF_TYPE_SPI :  
            if( temp <= 1000 && temp >= 10) {  
                SPI_Init(  temp * 1000 );    
                APP_TRACE_INFO(("\r\nSPI port is re-initialized to %d kHz \r\n",temp));        
            }  else {
                APP_TRACE_INFO(("\r\nERROR: SPI speed not support %d kHz\r\n",temp));
                err= SET_SPI_ERR ;
            } 
             
        break ;
//        case UIF_TYPE_GPIO :       
//   
//        break ;
        
        default:
             return UIF_TYPE_NOT_SUPPORT;
        break;
    }
    
    return err ; 
    
    
}


unsigned char Write_Single( SINGLE_WRITE single_write )
{  
    
    unsigned char state, err;
    
//    APP_TRACE_INFO(("\r\nWrite_Single: if_type=%d, dev_addr=0x%02X, reg_addr=0x%04X, reg_addr_len=%d, data=0x%04X, data_len=%d\r\n",\
//                         single_write.if_type,single_write.dev_addr,single_write.reg_addr,single_write.reg_addr_len,single_write.data,single_write.data_len));
//   
    
    err = NO_ERR;
    Reverse_Endian((unsigned char *)&single_write.reg_addr, single_write.reg_addr_len );
    Reverse_Endian( (unsigned char *)&single_write.data, single_write.data_len );
    
    switch( single_write.if_type ) {
        
        case UIF_TYPE_I2C:
              state =  TWID_Write( single_write.dev_addr>>1, 
                                  single_write.reg_addr, 
                                  single_write.reg_addr_len, 
                                  (unsigned char *)&single_write.data, 
                                  single_write.data_len, 
                                  NULL );       
              
              if (state != SUCCESS) {
                  err = I2C_BUS_ERR;
                  
              } 
        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( &single_write.data, 
                                            single_write.data_len );       
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
        break;
        
        case UIF_TYPE_GPIO:

        break; 
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
    }
        
    return err;   
    
    
}

unsigned char Read_Single( SINGLE_READ single_read )
{  
    
    unsigned char err, state;
    unsigned char *pbuf;
    
//    APP_TRACE_INFO(("\r\nRead_Single:  if_type=%d, dev_addr=0x%02X, reg_addr=0x%04X, reg_addr_len=%d, data_len=%d\r\n",\
//                         single_read.if_type,single_read.dev_addr,single_read.reg_addr,single_read.reg_addr_len,single_read.data_len));
//    
    err = NO_ERR;
    pbuf = (unsigned char *)Reg_RW_Data;
    
    Reverse_Endian((unsigned char *)&single_read.reg_addr, single_read.reg_addr_len );
    
    switch( single_read.if_type ) {
        
        case UIF_TYPE_I2C:
              state =  TWID_Read( single_read.dev_addr>>1, 
                        single_read.reg_addr, 
                        single_read.reg_addr_len, 
                        pbuf, 
                        single_read.data_len, 
                        NULL );     
    
              if (state != SUCCESS) {
                  err = I2C_BUS_ERR;
              }   
        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( pbuf, 
                                            single_read.data_len );       
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
        break;
        
//        case UIF_TYPE_GPIO:
//
//        break;
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
        
    }         
              
    Reverse_Endian(pbuf, single_read.data_len );
    
    APP_TRACE_INFO(("\r\nSingle Read[%0X] : %0X ", single_read.reg_addr, *(unsigned int*)pbuf));
      
    return err ;  
    
    
}


unsigned char Write_Burst( BURST_WRITE burst_write )
{  
    
    unsigned char state, err;
    
    APP_TRACE_INFO(("\r\nWrite_Single: if_type=%d, data_len=%d\r\n",\
                         burst_write.if_type,burst_write.data_len));
   
    
//    unsigned char *pdata = burst_write.pata;
//    APP_TRACE_INFO(("\r\nBurst Write Data:\r\n"));
//    for(unsigned int i = 0; i<burst_write.data_len; i++ ) {        
//        APP_TRACE_INFO(("0x%02X ",*pdata++));
//        if( i%10 == 9 ){
//            APP_TRACE_INFO(("\r\n"));
//        }
//    }
                    
    err = NO_ERR;
    //Reverse_Endian((unsigned char *)&single_write.reg_addr, single_write.reg_addr_len );
    //Reverse_Endian( (unsigned char *)&single_write.data, single_write.data_len );
    
    switch( burst_write.if_type ) {
        
//        case UIF_TYPE_I2C:
//              state =  TWID_Write( single_write.dev_addr>>1, 
//                                  single_write.reg_addr, 
//                                  single_write.reg_addr_len, 
//                                  (unsigned char *)&single_write.data, 
//                                  single_write.data_len, 
//                                  NULL );       
//              
//              if (state != SUCCESS) {
//                  err = I2C_BUS_ERR;
//                  
//              } 
//        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( burst_write.pata, 
                                            burst_write.data_len );       
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
        break;
        
//        case UIF_TYPE_GPIO:
//
//        break; 
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
    }
        
    return err;
    
}



unsigned char Read_Burst( BURST_READ burst_read )
{  
    
    unsigned char state, err;
    
    APP_TRACE_INFO(("\r\nRead_Single: if_type=%d, data_len=%d\r\n",\
                         burst_read.if_type,burst_read.data_len));
   
    
//    unsigned char *pdata = burst_write.pata;
//    APP_TRACE_INFO(("\r\nBurst Write Data:\r\n"));
//    for(unsigned int i = 0; i<burst_write.data_len; i++ ) {        
//        APP_TRACE_INFO(("0x%02X ",*pdata++));
//        if( i%10 == 9 ){
//            APP_TRACE_INFO(("\r\n"));
//        }
//    }
                    
    err = NO_ERR;
    //Reverse_Endian((unsigned char *)&single_write.reg_addr, single_write.reg_addr_len );
    //Reverse_Endian( (unsigned char *)&single_write.data, single_write.data_len );
    
    switch( burst_write.if_type ) {
        
//        case UIF_TYPE_I2C:
//              state =  TWID_Write( single_write.dev_addr>>1, 
//                                  single_write.reg_addr, 
//                                  single_write.reg_addr_len, 
//                                  (unsigned char *)&single_write.data, 
//                                  single_write.data_len, 
//                                  NULL );       
//              
//              if (state != SUCCESS) {
//                  err = I2C_BUS_ERR;
//                  
//              } 
//        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( burst_write.pata, 
                                            burst_write.data_len );       
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
        break;
        
//        case UIF_TYPE_GPIO:
//
//        break; 
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
    }
        
    return err;
    
}