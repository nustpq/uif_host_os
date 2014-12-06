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
#ifndef __UIF_H__
#define __UIF_H__


#define   UIF_TYPE_I2C     1
#define   UIF_TYPE_SPI     2
#define   UIF_TYPE_GPIO    3


#define   ATTRI_IM401_LOAD_CODE   41


extern unsigned char Reg_RW_Data[];

extern void Reverse_Endian( unsigned char *pdata, unsigned char size ) ;
extern unsigned char Setup_Interface( INTERFACE_CFG *interface_cfg );
extern unsigned char Raw_Write( RAW_WRITE *p_raw_write );
extern unsigned char Raw_Read( RAW_READ *p_raw_read );
extern unsigned char Write_Burst( BURST_WRITE nurst_write );
extern void Dump_Data ( unsigned char *pdata, unsigned int size );


#endif
