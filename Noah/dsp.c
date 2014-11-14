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
*                                        FM36 DSP Based Signal Path Setup
*
*                                          Atmel AT91SAM7A3
*                                               on the
*                                      Unified EVM Interface Board
*
* Filename      : dsp.c
* Version       : V1.0.0
* Programmer(s) : PQ
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


#include <includes.h>



static void Revert_patch_Endien( unsigned int *pData )
{
    
   unsigned char temp, *pChar;    
   
   pChar  = (unsigned char *)pData;   
   temp   = *pChar;
   *pChar = *(pChar+2);
   *(pChar+2) = temp;
   
}


////////////////////    for FM36-36D0 on AB01          /////////////////////////

static unsigned int fm36_patch_code_1[] = 
{
      0x003F00,  
      0x92FFFA,
      0x82266A,
      0x93FEDA,
      0x83FCFA,           
      0x23AA5F,           
      0x23B29F,           
      0x93FCFA,           
      0x82267A,           
      0x93FD5A,           
      0x82268A,            
      0x93FD6A,           
      0x82269A,           
      0x93FCCA,           
      0x8226AA,           
      0x93FCDA,           
      0x8226BA,           
      0x93FCBA,           
      0x83FD7A,           
      0x23A21F,           
      0x23A25F,           
      0x93FD7A,           
      0x82FFFA,           
      0x93F90A,           
      0x18466F  
};

static unsigned short int fm36_para_table_1[][2] = 
{   
  {0x3FA0, 0x8437},
  {0x3FB0, 0x0452},
  {0x3FA1, 0x8465},
  {0x3FB1, 0x3F00},
  
  //{0x2260, 0x78df},//16bit TDM
  //{0x2260, 0x78ff},//32bit TDM
  //{0x2260, 0x7AFF},//32bit TDM,MSB first,Left alignment
  {0x2260, 0x7AFD},//32bit TDM,MSB first,Left alignment
  {0x2266, 0x0013}, //3.072Hz
  //{0x2266, 0x001B}, //2.048M
  {0x2267, 0xBB80}, //3.072Hz
  //{0x2267, 0x7d00}, //2.048M
  {0x2268, 0xBB80}, //48k
  
  {0x2264, 0x01A0},
  {0x2265, 0x0000},
  {0x2288, 0x0000},
  {0x2289, 0x7FFF},
  {0x2290, 0x7FFF},
  {0x22FD, 0x009E},

  {0x2273, 0x0AAA},
  //{0x2274, 0x0001},//mic revert
  {0x2303, 0x8000},
  {0x2304, 0x4000},
  {0x2305, 0x0000},
  {0x226E, 0x0006},//OSC= 12.288MHz. PLL
  {0x2272, 0x003F},
  {0x2278, 0x0000},
  {0x22C1, 0x3FD9},
  {0x22C2, 0x3FDB},
  {0x22C3, 0x3FDD},
  {0x22C4, 0x3FDF},
  {0x22C5, 0x3FE1},
  {0x22C6, 0x3FE3}, 

  {0x22FB, 0     },
//  {0x3FCF, 0x0220}, //Bit9 : Enable inner LDO. Bit5: Enable PDM ADC CLK 
//  {0x3FED, 0x0013}, //set PDM CLK = 3.072MHz
//    
//  //fix PDM issue
//  {0x3F90, 0xFF }, //power down mic
//  {0x3F90, 0x00 }, //power down mic  

} ;


/////////////////////    for FM36-600C on AB02     /////////////////////////////

static unsigned int fm36_patch_code_2[] = 
{
    0x003F00,  
    0x3C0065,
    0x43700A,
    0x377201,
    0x17F05E,
    0x6800A5,
    0x22629F,
    0x44000A,
    0x19E78F,
    0x81012A,
    0x877200,
    0x6800A1,
    0x977200,
    0x81013A,
    0x877220,
    0x6800A1,
    0x977220,
    0x81014A,
    0x877240,
    0x6800A1,
    0x977240,
    0x81011A,
    0x19BFCF,
    0x877210,
    0x6000A1,
    0x977210,
    0x9101DA,
    0x877230,
    0x6000A1,
    0x977230,
    0x9101EA,
    0x877250,
    0x6000A1,
    0x977250,
    0x9101FA,
    0x862450,
    0x19C27F
};

static unsigned short int fm36_para_table_2[][2] = 
{
   
  {0x3FA0, 0x9E77},
  {0x3FB0, 0x3F00},
  {0x3FA1, 0x9BFB},
  {0x3FB1, 0x3F08},
  {0x3FA2, 0x9C26},
  {0x3FB2, 0x3F16},  

  //{0x2260, 0x78df},//16bit TDM
  //{0x2260, 0x78ff},//32bit TDM
  //{0x2260, 0x7AFF},//32bit TDM,MSB first,Left alignment,8 slot 
  {0x2260, 0x7AFD},//32bit TDM,MSB first,Left alignment,6 slot 
  {0x2266, 0x0013}, //3.072Hz
  //{0x2266, 0x001B}, //2.048M
  {0x2267, 0xBB80}, //3.072Hz
  //{0x2267, 0x7d00}, //2.048M 
  
  {0x2268, 0xBB80}, 
  {0x2264, 0x01A0},
  {0x2265, 0x0000},
  {0x2288, 0x0000},
  {0x2289, 0x7FFF},
  {0x2290, 0x7FFF},
  {0x22FD, 0x009E},  

  //{0x2274, 0x0001},//mic revert
  
  //{0x2303, 0x8000},
  //{0x2304, 0x4000},
  //{0x2305, 0x0000},
  {0x22FC, 0x8000},
  {0x226E, 0x0006}, //PLL  12.288MHz OSC 
  
  {0x22D7, 0x0000},
  {0x22D8, 0x0001},
  {0x22D9, 0x0002},
  {0x22DA, 0x0003},
  {0x22DB, 0x0004},
  {0x22DC, 0x0005}, 
  
  {0x22B2, 0x0001},
  {0x22B3, 0x0001},
  {0x22B4, 0x0001},  

  {0x2282, 0x0000},
  {0x2283, 0x0001},
  {0x2284, 0x0002},
  {0x2285, 0x0003},
  {0x2286, 0x0004},
  {0x2287, 0x0005}, 
  
  {0x22EB, 0x0006}, 
  
  {0x22C1, 0x101A},
  {0x22C2, 0x101B},
  {0x22C3, 0x101C},
  {0x22C4, 0x101D},
  {0x22C5, 0x101E},
  {0x22C6, 0x101F},
  
  //{0x2278, 0x0000}, //MIC Gain = 0dB
  
  {0x22FB, 0 },  
  
  {0x3F91, 0x0000}, //MEMS MIC TYPE
  //{0x3F92, 0x0000} //MEMS MIC gain = 0dB
  //{0x3F92, 0x0155} //MEMS MIC gain = 6dB
  //{0x3F92, 0x0AAA} //MEMS MIC gain = 12dB //SW default setting
  //{0x3F92, 0x03FF} //MEMS MIC gain = 18dB
  

} ;



/*
  //{0x3FC6, 0} //MIC0/1 volume
  //{0x3FC7, 0} //MIC2/3 volume
  //{0x3FC8, 0} //MIC4/5 volume
  //the real volume is volume/100
unsigned char Set_Mic_Volume( unsigned float vol )
{
    
    if( vol <= 0 && vol >= -143.25 ) {
        temp 
    } else if( vol <= 24 && vol >0 ) {
        
    } else {
        
    }
    
}
*/



////////////////////////    for FM36-600C on AB03      ////////////////////////

static unsigned int fm36_patch_code_3[] = 
{
    0x003F00,  
    0x3C0065,  
    0x43700A,  
    0x377201,  
    0x17F05E,  
    0x6800A5,  
    0x22629F,  
    0x3C0045,  
    0x43730A,  
    0x377261,  
    0x17F0BE,  
    0x6800A5,  
    0x22629F,  
    0x44000A,  
    0x19E78F,  
    0x81012A,  
    0x877200,  
    0x6800A1,  
    0x977200,  
    0x81013A,  
    0x877220,  
    0x6800A1,  
    0x977220,  
    0x81014A,  
    0x877240,  
    0x6800A1,  
    0x977240,  
    0x81011A,  
    0x19BFCF,  
    0x877210,  
    0x6000A1,  
    0x977210,  
    0x9101DA,  
    0x877230,  
    0x6000A1,  
    0x977230,  
    0x9101EA,  
    0x877250,  
    0x6000A1,  
    0x977250,  
    0x9101FA,  
    0x862450,  
    0x19C27F,  
    0x8A2C72,  
    0x7000AA,  
    0x877260,  
    0x6800A1,  
    0x977260,  
    0x8A2C82,  
    0x7000AA,  
    0x877280,  
    0x6800A1,  
    0x977280,  
    0x810BF1,  
    0x1831DF,  
    0x43F005,  
    0x3C0025,  
    0x3A2D50,  
    0x3B72A2,  
    0x1DC53F,  
    0x43F005,  
    0x19C8CF,  
    0x877270,  
    0x340088,  
    0x6000A1,  
    0x8B72A0,  
    0x7800A2,  
    0x977270,  
    0x877290,  
    0x6000A1,  
    0x8B72B0,  
    0x7800A2,  
    0x977290,  
    0x862550,  
    0x19A30F

};

//parameters for FM36 signal path on AB03
static unsigned short int fm36_para_table_3[][2] = 
{
  //patch settings
  {0x3FA0, 0x9E77},
  {0x3FB0, 0x3F00},
  {0x3FA1, 0x9BFB},
  {0x3FB1, 0x3F0E},
  {0x3FA2, 0x9C26},
  {0x3FB2, 0x3F1C},
  {0x3FA3, 0x831C},
  {0x3FB3, 0x3F2A},
  {0x3FA4, 0x9C8B},
  {0x3FB4, 0x3F36},
  {0x3FA5, 0x9A2F},
  {0x3FB5, 0x3F3D},    

  //////////////////////////////////////////////////////////////
  
  //SP0 port setting to 
  //{0x2260, 0x78df},//16bit TDM
  //{0x2260, 0x78ff},//32bit TDM
  {0x2260, 0x7AFF},//32bit TDM,MSB first,Left alignment,8 slot 
  //{0x2260, 0x7AFD},//32bit TDM,MSB first,Left alignment,6 slot 
  {0x2266, 0x0013}, //3.072Hz
  //{0x2266, 0x001B}, //2.048M
  {0x2267, 0xBB80}, //3.072Hz
  //{0x2267, 0x7d00}, //2.048M 
  
  //SP1 port setting
  {0x2261, 0x78FF},//32bit TDM,MSB first,Left alignment,8 slot, Left alignment of word inside slot to meet AD1937

  {0x2268, 0xBB80}, 
  {0x2264, 0x01A0},
  {0x2265, 0x0000},
  {0x2288, 0x0000},
  {0x2289, 0x7FFF},
  {0x2290, 0x7FFF},
  {0x22FD, 0x009E},  

  //{0x2274, 0x0001},//mic revert  
  //{0x2303, 0x8000},
  //{0x2304, 0x4000},
  //{0x2305, 0x0000},
  {0x22FC, 0x8000}, //BYPASS ALL SIGNAL PORCESSING
  {0x226E, 0x001C}, //PLL 24.576MHz OSC 
  
  ////////////////////////////////////////////////////////////
  
  //select data interruption source :  
  // 0: PDM, 1: SP0, 2: SP1, 3: SP2
  {0x22B2, 0x0001},
  {0x22B3, 0x0001},
  {0x22B4, 0x0001}, 
  
    //additional, input
    ////229A = 0x1 //Aux-in From SP0
    ////229B = 0x0 //Aux-in-L in slot0 
    ////229C = 0x1 //Aux-in-R in slot1
    //// 
    //////output, aux2 output same as aux1
    ////22C7 = 0x1018 //Aux-in-L
    ////22C8 = 0x1019 //Aux-in-R
    ////22B4 = 1 //SP0
    ////22D5 = 6 //slot6 
    ////22D6 = 7 //slot7
  
  //lin source 
  {0x229A, 0x0002}, //Aux-in From SP1
  {0x229B, 0x0008}, //Aux-in-L in SP1 slot0 
  {0x229C, 0x0009}, //Aux-in-R in SP1 slot1
  
  //select output data source slot
  {0x22C1, 0x101A},
  {0x22C2, 0x101B},
  {0x22C3, 0x101C},
  {0x22C4, 0x101D},
  {0x22C5, 0x101E},
  {0x22C6, 0x101F},   
//  {0x22C7, 0x1018}, //Aux-in-L
//  {0x22C8, 0x1019}, //Aux-in-R
  {0x22C7, 0x1020}, //default 0
  {0x22C8, 0x1020}, //default 0
  
  //select data dest slot
  //If lineout is from TX0, offset is 0~7
  //If lineout is from TX1, offset is 8~15
  //If lineout is from TX2, offset is 16~23
  //If offset = 0x8000, means Zero output.
  {0x22D7, 0x0000},
  {0x22D8, 0x0001},
  {0x22D9, 0x0002},
  {0x22DA, 0x0003},
  {0x22DB, 0x0004},
  {0x22DC, 0x0005},  
  {0x22D5, 0x0006}, //slot6 
  {0x22D6, 0x0007}, //slot7
  
  //mic souce
  {0x2282, 0x0000},
  {0x2283, 0x0001},
  {0x2284, 0x0002},
  {0x2285, 0x0003},
  {0x2286, 0x0004},
  {0x2287, 0x0005}, 
  
  {0x22EB, 0x0006}, 
  
  {0x22FB, 0 }, 
  
  /*******************  MIC Volume Control  ****************
  Volume setting [7:4]:
  0x0 ( 0 dB) 0x4 (-48 dB) 0x8 (-96 dB) 0xc (Not allowed)
  0x1 (-12 dB) 0x5 (-60 dB) 0x9 (-108 dB) 0xd (Not allowed)
  0x2 (-24 dB) 0x6 (-72 dB) 0xa (-120 dB) 0xe (+24 dB)
  0x3 (-36 dB) 0x7 (-84 dB) 0xb (-132 dB) 0xf (+12 dB)
  Volume setting [3:0]:
  0x0 ( 0 dB) 0x4 (-3.00 dB) 0x8 (-6.00 dB) 0xc (-9.00 dB)
  0x1 (-0.75 dB) 0x5 (-3.75 dB) 0x9 (-6.75 dB) 0xd (-9.75 dB)
  0x2 (-1.50 dB) 0x6 (-4.50 dB) 0xa (-7.50 dB) 0xe (-10.50 dB)
  0x3 (-2.25 dB) 0x7 (-5.25 dB) 0xb (-8.25 dB) 0xf (-11.25 dB)
  Real volume = Volume for [7:4] + Volume for [3:0]
  **********************************************************/  
  //{0x3FC6, 0x0000}, //MIC0,1 Volume Gain = 0dB
  //{0x3FC7, 0x0000}, //MIC2,3 Volume Gain = 0dB
  //{0x3FC8, 0x0000}, //MIC4,5 Volume Gain = 0dB
  
  /**************** MEMS Gain Control **********************/
  //{0x3F91, 0x0000}, //MEMS MIC TYPE, SW default setting as MEMS type
  //{0x3F92, 0x0000} //MEMS MIC gain = 0dB
  //{0x3F92, 0x0555} //MEMS MIC gain = 6dB
  //{0x3F92, 0x0AAA} //MEMS MIC gain = 12dB //SW default setting
  //{0x3F92, 0x0FFF} //MEMS MIC gain = 18dB

} ;

//DMIC 4 steps PGA gain
unsigned char DMIC_PGA_Control( unsigned short gain ) 
{
    unsigned char  err  ;
    unsigned short data ;
    unsigned short mute ;
    
    mute = 0x3F;
    err = NO_ERR;
    
    switch ( gain ){
        case 0 : 
            data = 0x0000;
        break;
        case 6 : 
            data = 0x0555;
        break;
        case 12 : 
            data = 0x0AAA;
        break;
        case 18 : 
            data = 0x0FFF;
        break;
        case 1000 :
            mute = 0x3F3F;
        break;
        default:
            return FM36_DMIC_PGA_GAIN_ERR;
    }  
    
    if( mute == 0x3F ) {
      err = DM_SingleWrite( FM36_I2C_ADDR, 0x3F92, data ) ;
      if( OS_ERR_NONE != err ) {
          return FM36_WR_DM_ERR;;
      }   
    }    
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x3F96, mute ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }    
    
    return err;
    
}


//Setup External Lin data input source
//From : SP0_RX, SP1_RX
static unsigned char Config_Lin( unsigned char lin_sp_index, unsigned char start_slot_index )
{
    
    unsigned char err ;
   
    //Aux-Source
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x229A, lin_sp_index+1 ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }
    
    //Aux-in-L
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x229B, (lin_sp_index<<3) + start_slot_index ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }
    
    //Aux-in-R
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x229C, (lin_sp_index<<3) + start_slot_index + 1 ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }    
   
    return err;
    
}

// Set additional data besides MIC data
// M0..M5 + X0 + X1
// Max 8 slots
// Make sure last 2 slots folowed mic data closely
static unsigned char Config_SP0_Out( unsigned char mic_num )
{
    
    unsigned char err ;
  
    //select output data source slot
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x22C1 + mic_num, 0x1018 ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }       
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x22C2 + mic_num, 0x1019 ) ;
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }     
  
    return err;
    
}


static unsigned char Config_SPx_Format( unsigned char bit_length )
{
    
    unsigned char err ;
    unsigned char temp;
    
    if( bit_length == 32 ) {
          temp = 0x7B7F;//32bit TDM, 24bit data, MSB first,Left alignment,8 slot 
          //temp = 0x7AFF;//32bit TDM, 16bit data, MSB first,Left alignment,8 slot 
    } else {
          temp = 0x7B5F;//16bit TDM,16bit data, MSB first,Left alignment,8 slot 
    }
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x2260, temp ) ; //SP0
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    }       
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x2261, temp ) ; //SP1
    if( OS_ERR_NONE != err ) {
        return FM36_WR_DM_ERR;;
    } 
    
    return err;
    
}   

  
  

//Note: This routine do NOT support reentrance
static unsigned int sr_saved = 0;
static unsigned int mic_num_saved = 0;

//Just Change the sample rate of FM36
unsigned char ReInit_FM36( unsigned short sr )
{
   
    unsigned char  err;
    unsigned short temp;
    
    if( sr == sr_saved ) {
        return NO_ERR;
    } else {
        sr_saved = sr ;
    }  
   
    switch ( sr ) {
        case 8000 :
            temp = 0x1F40;
        break;        
        case 16000 :
            temp = 0x3E80;
        break;
        case 24000 :
            temp = 0x5DC0;
        break;
        case 32000 :
            temp = 0x7D00;
        break;
        case 44100 :
            temp = 0xAC44;
        break;
        default: // 48000 :
            temp = 0xBB80;
        break;   
    }
    
    err = DM_SingleWrite( FM36_I2C_ADDR, 0x3FD6, temp ) ;
    if( OS_ERR_NONE != err ) {
        err = FM36_WR_DM_ERR;
    }
    return err ; 
    
}


unsigned char Init_FM36( unsigned short sr )
{
    unsigned int i;
    unsigned short temp, temp2 ;
    unsigned short addr, val; 
    unsigned char err ;
    unsigned char dsp_type;
    
    
    if( sr == sr_saved ) {
        return 0;
    } else {
        sr_saved = sr ;
    }    
   
    Pin_Reset_FM36();  

    err = HOST_SingleWrite_2(FM36_I2C_ADDR,0x0C, 2); //reset
    if( OS_ERR_NONE != err ) {
        return err ;
    }
     
//    err = DM_LegacyRead( FM36_I2C_ADDR, 0x22FB,(unsigned char *)&temp ) ;
//    if( OS_ERR_NONE != err ) {
//        return err ;
//    }  
//    APP_TRACE_INFO(("0x22FB = 0x%X\r\n", temp)); 
//    
//    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp ) ;
//    if( OS_ERR_NONE != err ) {
//        return err ;
//    }  
//    APP_TRACE_INFO(("0x2306 = 0x%X\r\n", temp)); 
    
    
    err = CM_LegacyRead( FM36_I2C_ADDR, 0x2FFF,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        return err ;
    }
    APP_TRACE_INFO(("ROM ID = 0x%X\r\n", temp)); 
    if( temp == FM36_ROM_ID_1 ) {
        dsp_type = 0;
    } else if ( temp == FM36_ROM_ID_2 ) {
        dsp_type = 1;
    } else {
        return (0xA0); //dsp type error
    } 
       
    // set SR
    switch ( sr ) {
        case 8000 :
            temp = 0x1F40;
        break;        
        case 16000 :
            temp = 0x3E80;
        break;
        case 24000 :
            temp = 0x5DC0;
        break;
        case 32000 :
            temp = 0x7D00;
        break;
        case 44100 :
            temp = 0xAC44;
        break;
        default: // 48000 :
            temp = 0xBB80;
        break;   
    }
 
    if( dsp_type == 0 )  { 
        //patch code
        addr = (unsigned short)fm36_patch_code_1[0];
        for( i = 1; i < sizeof(fm36_patch_code_1)/4; i++) { 
          Revert_patch_Endien(&fm36_patch_code_1[i]);    
          err = PM_SingleWrite( FM36_I2C_ADDR, addr++, (unsigned char*)&fm36_patch_code_1[i],0 ) ;
            if( OS_ERR_NONE != err ) {
                return err ;
            }    
        }    
        //parameters
        for( i = 0; i < sizeof(fm36_para_table_1)/4; i++) {            
            addr = fm36_para_table_1[i][0];
            val  = fm36_para_table_1[i][1];
            if( addr == 0x2268 ) { //SR
                val = temp ;
            }
            err = DM_SingleWrite( FM36_I2C_ADDR, addr, val ) ;
            if( OS_ERR_NONE != err ) {
                return err ;
            }
            if( addr == 0x22FB ) {  //run chip
                OSTimeDly(100);
            }
        }        
    
    } else {
        //patch code
        addr = (unsigned short)fm36_patch_code_2[0];
        for( i = 1; i < sizeof(fm36_patch_code_2)/4; i++) { 
          Revert_patch_Endien(&fm36_patch_code_2[i]);    
          err = PM_SingleWrite( FM36_I2C_ADDR, addr++, (unsigned char*)&fm36_patch_code_2[i],0 ) ;
            if( OS_ERR_NONE != err ) {
                return err ;
            }         
        } 
        //parameters
        for( i = 0; i < sizeof(fm36_para_table_2)/4; i++) {            
            addr = fm36_para_table_2[i][0];
            val  = fm36_para_table_2[i][1];
            if( addr == 0x2268 ) { //SR
                val = temp ;
            }
            err = DM_SingleWrite( FM36_I2C_ADDR, addr, val ) ;
            if( OS_ERR_NONE != err ) {
                return err ;
            }
            if( addr == 0x22FB ) {  //run chip
                OSTimeDly(100);
            } 

        } 
        
    }

    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp2 ) ;
    if( OS_ERR_NONE != err ) {
        return err ;
    }
    OSTimeDly(100);
    err = DM_LegacyRead( FM36_I2C_ADDR, 0x22FB,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        return err ;
    }
    APP_TRACE_INFO(("0x22FB = 0x%X\r\n", temp));
    if( temp != 0x5A5A ) {   
        return (2);
    }
    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        return err ;
    }
    if( temp == temp2 ) {
        APP_TRACE_INFO(("frame counter stopped !"));
        return (3);
    }    
    return (err);
    
}

static bool flag_power_lose = true;

/*
*********************************************************************************************************
*                                       Init_FM36_AB03()
*
* Description : Initialize FM36 DSP on AB03 board.
* Argument(s) : sr        : sample rate : 8000 ~ 48000 
*               bit_length: 16bit/32bit mode
*               mic_num   : 0~6
*               lin_sp_index  : line in data source: 0 ~ 1
*               start_slot_index: line in data slot: 0 ~ 7
* Return(s)   : NO_ERR :   execute successfully
*               others :   =error code .  
*
* Note(s)     : None.
*********************************************************************************************************
*/
unsigned char Init_FM36_AB03( unsigned short sr, unsigned char bit_length, unsigned char mic_num, unsigned char lin_sp_index, unsigned char start_slot_index )
{
    unsigned int   i;
    unsigned short temp, temp2 ;
    unsigned short addr, val; 
    unsigned char  err ;      
    
    if( sr == sr_saved  &&  mic_num == mic_num_saved ) { //just check this 2 parameters, b/c other won't change in AB03
        return NO_ERR;
    } else {
        sr_saved = sr ;
        mic_num_saved = mic_num ;
    }   
    
    Pin_Reset_FM36();  
    
//    err = HOST_SingleWrite_2(FM36_I2C_ADDR, 0x0C, 2); //reset
//    if( OS_ERR_NONE != err ) {
//        return FM36_WR_HOST_ERR;
//    }    
//    err = DM_LegacyRead( FM36_I2C_ADDR, 0x22FB,(unsigned char *)&temp ) ;
//    if( OS_ERR_NONE != err ) {
//        err = FM36_RD_DM_ERR;
//        return err ;
//    }  
//    APP_TRACE_INFO(("0x22FB = 0x%X\r\n", temp));     
//    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp ) ;
//    if( OS_ERR_NONE != err ) {
//        err = FM36_RD_DM_ERR;
//        return err ;
//    }  
//    APP_TRACE_INFO(("0x2306 = 0x%X\r\n", temp));     
    
    err = CM_LegacyRead( FM36_I2C_ADDR, 0x2FFF,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        err = FM36_RD_CM_ERR;
        return FM36_RD_CM_ERR ;
    }
    APP_TRACE_INFO(("ROM ID = 0x%X\r\n", temp));
    
    if( temp != FM36_ROM_ID_2 ) { 
        return FM36_CHECK_ROMID_ERR; //dsp type error
    } 
       
    // set SR
    switch ( sr ) {
        case 8000 :
            temp = 0x1F40;
        break;        
        case 16000 :
            temp = 0x3E80;
        break;
        case 22050 :
            temp = 0x5622;
        break;
        case 24000 :
            temp = 0x5DC0;
        break;
        case 32000 :
            temp = 0x7D00;
        break;
        case 44100 :
            temp = 0xAC44;
        break;
        default: //48000 :
            temp = 0xBB80;
        break;   
    }
 
    //FM36-600C
    //patch code
    if( flag_power_lose ) { //only need download patch once after one power cycle
            
        flag_power_lose = false;
        addr = (unsigned short)fm36_patch_code_3[0];
        for( i = 1; i < sizeof(fm36_patch_code_3)/4; i++) { 
            Revert_patch_Endien(&fm36_patch_code_3[i]);    
            err = PM_SingleWrite( FM36_I2C_ADDR, addr++, (unsigned char*)&fm36_patch_code_3[i],0 ) ;
              if( OS_ERR_NONE != err ) {
                  err = FM36_WR_PM_ERR;
                  return err ;
              }         
        }
    } 
    //parameters
    for( i = 0; i < sizeof(fm36_para_table_3)/4; i++) {            
        addr = fm36_para_table_3[i][0];
        val  = fm36_para_table_3[i][1];
        if( addr == 0x2268 ) { //SR
            val = temp ;
        }
        if( addr == 0x22FB ) {  //if run chip, do sth before
            err = Config_SP0_Out( mic_num );
            if( OS_ERR_NONE != err ) {
                return err ;
            }
            err = Config_Lin( lin_sp_index, start_slot_index );
            if( OS_ERR_NONE != err ) {
                return err ;
            }
            err = Config_SPx_Format( bit_length );
            if( OS_ERR_NONE != err ) {
                return err ;
            }
            
        } 
        err = DM_SingleWrite( FM36_I2C_ADDR, addr, val ) ;
        if( OS_ERR_NONE != err ) {
            err = FM36_WR_DM_ERR;
            return err ;
        }
        if( addr == 0x22FB ) {  //if run chip, delay
            OSTimeDly(100);            
        } 
    } 
          
    
    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp2 ) ;
    if( OS_ERR_NONE != err ) {
        err = FM36_RD_DM_ERR;
        return err ;
    }
    APP_TRACE_INFO(("0x2306 = 0x%X\r\n", temp2));
    OSTimeDly(100);
    err = DM_LegacyRead( FM36_I2C_ADDR, 0x22FB,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        err = FM36_RD_DM_ERR;
        return err ;
    }
    APP_TRACE_INFO(("0x22FB = 0x%X\r\n", temp));
    if( temp != 0x5A5A ) {   
        return FM36_CHECK_FLAG_ERR;
    }
    err = DM_LegacyRead( FM36_I2C_ADDR, 0x2306,(unsigned char *)&temp ) ;
    if( OS_ERR_NONE != err ) {
        err = FM36_RD_DM_ERR;
        return err ;
    }
    APP_TRACE_INFO(("0x2306 = 0x%X\r\n", temp));
    if( temp == temp2 ) {
        APP_TRACE_INFO(("FM36 frame counter stopped !"));
        return FM36_CHECK_COUNTER_ERR;
    }    
    return err;
    
}


