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

extern unsigned char   Reg_RW_Data[];

extern void Reverse_Endian( unsigned char *pdata, unsigned char size ) ;
extern unsigned char Setup_Interface( INTERFACE_CFG *interface_cfg );
extern unsigned char Write_Single( SINGLE_WRITE single_write );
extern unsigned char Read_Single( SINGLE_READ single_read );
extern unsigned char Write_Burst( BURST_WRITE nurst_write );

#endif
