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


INTERFACE_CFG   Global_UIF_Setting[3];     //ruler type = 3
unsigned char   Reg_RW_Data[ EMB_BUF_SIZE ];


//disabled endian reverse, as PC scripts will do it!
void Reverse_Endian( unsigned char *pdata, unsigned char size ) 
{
    
    unsigned char i;
    unsigned char temp;
    
    for( i = 0 ; i< size>>1 ; i++ ) {
        
      temp = *( pdata+i );
      *( pdata+i ) = *( pdata + size -1 - i );
      *( pdata + size -1 - i ) = temp;
      
    }      
    
}

/*
*********************************************************************************************************
*                                           Dump_Data()
*
* Description : print data package on debug uart for debug .
* Argument(s) : *pdata : pointer to data address
*                 size : N bytes
*               
* Return(s)   : None.           
*
* Note(s)     : None.
*********************************************************************************************************
*/
void Dump_Data ( unsigned char *pdata, unsigned int size )
{
    unsigned int i ;
    
    if( size == 0 ) {  
        return; 
    }
    
    APP_TRACE_INFO(("\r\n---------------------- Dump Data(Hex) -------------------------"));
    
    for( i = 0; i < size ; i++ ) {         
        if( i%16 == 0 ) {
            APP_TRACE_INFO(("\r\n"));
        }
        APP_TRACE_INFO((" %02X ",*pdata++));
    }  
    
    APP_TRACE_INFO(("\r\n---------------------------------------------------------------\r\n"));

}

/*
*********************************************************************************************************
*                                           Setup_Interface()
*
* Description : Send command to .
* Argument(s) : INTERFACE_CFG : 
*               
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
    
    APP_TRACE_INFO(("\r\nSetup_Interface: if_type=%d, speed=%dkHz, attribute=0x%X ",\
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
                SPI_Init(  temp * 1000, pInterface_Cfg->attribute );    
                APP_TRACE_INFO(("\r\nSPI port is re-initialized to %d kHz \r\n",temp));        
            }  else {
                APP_TRACE_INFO(("\r\nERROR: SPI speed not support %d kHz\r\n",temp));
                err= SET_SPI_ERR ;
            } 
             
        break 
            ;
        case UIF_TYPE_GPIO :       
   
        break ;
        
        default:
             err = UIF_TYPE_NOT_SUPPORT;
        break;
    }
    
    if ( err == NULL ) {
        Global_UIF_Setting[ pInterface_Cfg->if_type - 1 ].attribute = pInterface_Cfg->attribute;
        Global_UIF_Setting[ pInterface_Cfg->if_type - 1 ].if_type   = pInterface_Cfg->if_type;
        Global_UIF_Setting[ pInterface_Cfg->if_type - 1 ].speed     = pInterface_Cfg->speed;
    }
    
    return err ; 
    
    
}


/*
*********************************************************************************************************
*                                           Dump_Data()
*
* Description : print data package on debug uart for debug .
* Argument(s) : *pdata : pointer to data address
*                 size : N bytes
*               
* Return(s)   : None.           
*
* Note(s)     : None.
*********************************************************************************************************
*/
unsigned char Raw_Write( RAW_WRITE *p_raw_write )
{  
    
    unsigned char  state, err;
    unsigned char  buf[5] ; 
    unsigned char *pChar;
    unsigned int   size;
    
    APP_TRACE_INFO(("\r\nRaw_Write: if_type=%d, dev_addr=0x%02X, data_len=%d ",\
                         p_raw_write->if_type,p_raw_write->dev_addr,p_raw_write->data_len));    
    //Dump_Data( p_raw_write->pdata,  p_raw_write->data_len );    
    
    err = NO_ERR;
    pChar  = p_raw_write->pdata ;
    
    switch( p_raw_write->if_type ) {
        
        case UIF_TYPE_I2C:  
        
            if( Global_UIF_Setting[p_raw_write->if_type - 1 ].attribute == ATTRI_IM401_LOAD_CODE ) {
                  
                OSTimeDly(1); 
                
                buf[0] = 0xF0;
                buf[1] = *pChar++;
                buf[2] = *pChar++;
                state =  TWID_Write( p_raw_write->dev_addr>>1, 0, 0, buf, 3,  NULL );                 
                if ( state != SUCCESS ) {
                    err = I2C_BUS_ERR;
                    break; 
                }
                buf[0] = 0xF1;
                buf[1] = *pChar++;
                buf[2] = *pChar++;
                state =  TWID_Write( p_raw_write->dev_addr>>1, 0, 0, buf, 3,  NULL );                  
                if ( state != SUCCESS ) {
                    err = I2C_BUS_ERR;
                    break; 
                }                    
                buf[0] = 0xF8; 
                size   = (p_raw_write->data_len-4)>>2 ;
                for( unsigned int i =0 ; i <size ; i++ ) {  
                    buf[1] = *pChar++;
                    buf[2] = *pChar++;
                    buf[3] = *pChar++;
                    buf[4] = *pChar++;
                    state =  TWID_Write( p_raw_write->dev_addr>>1, 0, 0, buf, 5,  NULL );               
                    if (state != SUCCESS) {
                        err = I2C_BUS_ERR;
                        break;                  
                    }                      
                    //OSTimeDly(1); 
                }
                
            } else  { 
                
              state =  TWID_Write( p_raw_write->dev_addr>>1, 0, 0, p_raw_write->pdata, p_raw_write->data_len, NULL );       
              if (state != SUCCESS) {
                  err = I2C_BUS_ERR;                  
              } 
              
            }
        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( p_raw_write->pdata, p_raw_write->data_len );              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
        break;
        
        case UIF_TYPE_GPIO:
             err = GPIOPIN_Set( p_raw_write->dev_addr, *(p_raw_write->pdata) );

        break; 
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
    }
        
    return err;   
    
    
}

/*
*********************************************************************************************************
*                                           Raw_Read()
*
* Description : read data, and some pre data can be write just before read .
* Argument(s) :  p_raw_read : pointer to RAW_READ type address
*                
*               
* Return(s)   : None.           
*
* Note(s)     : None.
*********************************************************************************************************
*/
unsigned char Raw_Read( RAW_READ *p_raw_read )
{  
    
    unsigned char err, state;
    unsigned char *pbuf;
    
    err  = NO_ERR;
    pbuf = (unsigned char *)Reg_RW_Data;
    
    APP_TRACE_INFO(("\r\nRaw_Read:  if_type=%d, dev_addr=0x%02X, data_len_read=%d, data_len_write=%d ",\
                         p_raw_read->if_type,p_raw_read->dev_addr,p_raw_read->data_len_read,p_raw_read->data_len_write ));
    
    Dump_Data( p_raw_read->pdata_write,  p_raw_read->data_len_write );
    
    switch( p_raw_read->if_type ) {
        
        case UIF_TYPE_I2C:
              state =  TWID_Write( p_raw_read->dev_addr>>1,
                                  0, 
                                  0, 
                                  p_raw_read->pdata_write, 
                                  p_raw_read->data_len_write, 
                                  NULL );     
              if (state != SUCCESS) {
                  err = I2C_BUS_ERR;
              } 
              
              state =  TWID_Read( p_raw_read->dev_addr>>1,
                                  0, 
                                  0, 
                                  pbuf, 
                                  p_raw_read->data_len_read, 
                                  NULL );     
              if (state != SUCCESS) {
                  err = I2C_BUS_ERR;
              } 
        break;
        
        case UIF_TYPE_SPI:      
        
              state =  SPI_ReadWriteBuffer_API(  pbuf, 
                                               p_raw_read->pdata_write, 
                                               p_raw_read->data_len_read + 1, 
                                               p_raw_read->data_len_write);// +1 fix SPI bug
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  APP_TRACE_INFO(("\r\nSPI_ReadBuffer_API err = %d",state));
              }              
              pbuf = pbuf + (1 + p_raw_read->data_len_write); //fix bug
        break;
        
        case UIF_TYPE_GPIO:
              err = GPIOPIN_Get( p_raw_read->dev_addr, pbuf );
        break;
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
        
    }       
    
    if( err != NO_ERR ) {
        APP_TRACE_INFO(("\r\nRaw_Read() failed: %d", err));
        
    } else {
        p_raw_read->pdata_read = pbuf ; 
        Dump_Data( pbuf,  p_raw_read->data_len_read ); 
        
    }
    
    return err ;      
    
}














unsigned char Write_Burst( BURST_WRITE burst_write )
{  
    
    unsigned char   state, err;
    unsigned char   buf[5] ;  
    unsigned char  *pchar;
    
    APP_TRACE_INFO(("\r\nWrite_Burst: if_type=%d, mem_addr=0x%02X:%02X, mem_addr_len=%d,data_len=%d ",\
                         burst_write.if_type,burst_write.mem_addr_h,burst_write.mem_addr_l,burst_write.mem_addr_len,burst_write.data_len));
   
    
//    unsigned char *pdata = (unsigned char *)burst_write.pdata;
//    APP_TRACE_INFO(("0x%04X ",pdata));
//    APP_TRACE_INFO(("\r\nBurst Write Data:\r\n"));
//    for(unsigned int i = 0; i<(burst_write.data_len); i++ ) {        
//        APP_TRACE_INFO(("0x%02X ",*pdata++));
//        if( i%10 == 9 ){
//            APP_TRACE_INFO(("\r\n"));
//        }
//    }
                    
    err = NO_ERR;
    //Reverse_Endian((unsigned char *)&raw_write.reg_addr, raw_write.reg_addr_len );
    //Reverse_Endian( (unsigned char *)&raw_write.data, raw_write.data_len );
    
    switch( burst_write.if_type ) {
        
        case UIF_TYPE_I2C:
                   if( Global_UIF_Setting[burst_write.if_type - 1 ].attribute == ATTRI_IM401_LOAD_CODE ) {
                     OSTimeDly(1);            
                   buf[0] = 0xF0;
                   buf[1] = burst_write.mem_addr_l & 0xFF;
                   buf[2] = (burst_write.mem_addr_l >> 8) & 0xFF;
                   state =  TWID_Write( burst_write.dev_addr>>1, 0, 0, buf, 3,  NULL );                 
                    if ( state != SUCCESS ) {
                        err = I2C_BUS_ERR;
                        break; 
                    }
                    buf[0] = 0xF1;
                    buf[1] = burst_write.mem_addr_h & 0xFF;
                    buf[2] = (burst_write.mem_addr_h >> 8) & 0xFF;
                    state =  TWID_Write( burst_write.dev_addr>>1, 0, 0, buf, 3,  NULL );                  
                    if ( state != SUCCESS ) {
                        err = I2C_BUS_ERR;
                        break; 
                    }                    
                    buf[0] = 0xF8;
                    pchar = burst_write.pdata;
                    for( unsigned int i =0 ; i <(burst_write.data_len>>2) ; i++ ) {  
                        buf[1] = *pchar++;
                        buf[2] = *pchar++;
                        buf[3] = *pchar++;
                        buf[4] = *pchar++;
                        state =  TWID_Write( burst_write.dev_addr>>1, 0, 0, buf, 5,  NULL );               
                        if (state != SUCCESS) {
                            err = I2C_BUS_ERR;
                            break;                  
                     }                      
                     //OSTimeDly(1); 
                    }
            
              } else {
//                    state =  TWID_Write( burst_write.dev_addr>>1, 
//                                  burst_write.mem_addr, 
//                                  burst_write.mem_addr_len, 
//                                  burst_write.pdata, 
//                                  burst_write.data_len, 
//                                  NULL );       
//              
//                    if (state != SUCCESS) {
//                        err = I2C_BUS_ERR;
//                  
//                    } 
              }
        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_WriteBuffer_API( burst_write.pdata, 
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
    
    APP_TRACE_INFO(("\r\nRead_Burst: if_type=%d, data_len=%d ",\
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
    //Reverse_Endian((unsigned char *)&raw_write.reg_addr, raw_write.reg_addr_len );
    //Reverse_Endian( (unsigned char *)&raw_write.data, raw_write.data_len );
    
    switch( burst_read.if_type ) {
        
//        case UIF_TYPE_I2C:
//              state =  TWID_Write( raw_write.dev_addr>>1, 
//                                  raw_write.reg_addr, 
//                                  raw_write.reg_addr_len, 
//                                  (unsigned char *)&raw_write.data, 
//                                  raw_write.data_len, 
//                                  NULL );       
//              
//              if (state != SUCCESS) {
//                  err = I2C_BUS_ERR;
//                  
//              } 
//        break;
        
        case UIF_TYPE_SPI:
              state =  SPI_ReadBuffer_API( burst_read.pdata, 
                                            burst_read.data_len );       
              
              if (state != SUCCESS) {
                  err = SPI_BUS_ERR;
                  
              } 
//        break;
        
//        case UIF_TYPE_GPIO:
//
//        break; 
        
        default:
             err = UIF_TYPE_NOT_SUPPORT ;             
        break;
    }
        
    return err;
    
}