/*
*********************************************************************************************************
*
*                                             EXAMPLE CODE
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          MASTER INCLUDE FILE
*
*                                          Atmel AT91SAM7X
*                                                on the
*                                  Atmel AT91SAM7X-EK Evaluation Board
*
* Filename      : app.c
* Version       : V1.0.0
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>

#include    <at91sam7a3.h>

#include    <ucos_ii.h>

#include    <cpu.h>
#include    <lib_def.h>
#include    <lib_mem.h>
#include    <lib_str.h>
#include    <lib_ascii.h>
#include    <lib_math.h>


#include    <bsp.h>
#include    <app_cfg.h>





#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) || \
    (APP_CFG_PROBE_COM_EN == DEF_ENABLED)
#include    <app_probe.h>
#endif
#endif
