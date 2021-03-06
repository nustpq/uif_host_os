#include <bsp.h>
#include <pio.h>
#include <async.h>
#include <twid.h>
#include "codec.h"



unsigned char Codec_Read(unsigned char dev_addr,unsigned char reg,unsigned char *pVal)
{
    unsigned char state = 0;   
    state =  TWID_Read( dev_addr>>1, reg, 1, pVal++,1, NULL) ;
    return(state);
}


unsigned char Codec_Write(unsigned char dev_addr,unsigned char reg,unsigned char data)
{      
    unsigned char buf[] = {  data };     
    unsigned char state;
      
    state =  TWID_Write( dev_addr>>1, reg, 1 , buf, sizeof(buf), NULL);     
    
    return state ;
}


unsigned char Codec_Read_SPI(unsigned char dev_addr,unsigned char reg,unsigned char *pVal)
{
    unsigned char state = 0;   
    //state =  TWID_Read( dev_addr>>1, reg, 1, pVal++,1, NULL) ;
    return(state);
}


unsigned char Codec_Write_SPI(unsigned char dev_addr,unsigned char reg,unsigned char data)
{      
    unsigned char buf[] = { dev_addr<<1, reg, data };     
    unsigned char state;    
    
    state = SPI_WriteBuffer_API( buf, 3);     
  
    
    return state ;
    
}


//i2c_channel = 0 ~ 7
unsigned char Codec_Mixer(unsigned char i2c_channel )
{    
     unsigned char buf   ;
     unsigned char state ;
     
     state = 0xe0 ; 
     
     if( i2c_channel <= 3 )  {//PCA9548A,
        buf   = 0x01<<i2c_channel ; //select i2c_channel
        state = TWID_Write( PCA9548A_ADDRESS>>1, 0, 0, &buf, 1, NULL); 
     /*
        buf   = 0 ;
        state = TWID_Write( PCA9540B_ADDRESS>>1, 0, 0, &buf, 1, NULL); 
      
     } else {  // PCA9540B
        buf   = 4 ; //select CH0
        state = TWID_Write( PCA9540B_ADDRESS>>1, 0, 0, &buf, 1, NULL); 
        buf   = 0 ;
        state = TWID_Write( PCA9548A_ADDRESS>>1, 0, 0, &buf, 1, NULL); 
     */
     }
     
     return state ;
}






unsigned char Set_Codec(unsigned char codec_control_type, unsigned char size_para, unsigned char *pdata)
{
    unsigned char i, state = 0 ;
    for(i = 0; i< size_para ; i++)
    {
      if(codec_control_type == 0 ) {
        state = Codec_Write(CODEC_ADDRESS,*(pdata+i*2),*(pdata+i*2+1));
      } else {
        state = Codec_Write_SPI(AD1938_ADDRESS,*(pdata+i*2),*(pdata+i*2+1));
      }
        if( state != 0 ) break;
    }
    return state;
  
}


unsigned char Get_Codec(unsigned char codec_control_type, unsigned char reg, unsigned char *pdata)
{
    unsigned char state = 0 ;
    
    if(codec_control_type == 0 ) {
        state = Codec_Read( CODEC_ADDRESS, reg, pdata) ;
    } else {
        state = Codec_Read_SPI( CODEC_ADDRESS, reg, pdata) ;
    }
    
    return state;
  
}


unsigned char I2CWrite_Codec( unsigned char reg,unsigned char data )
{
    unsigned char err;
    err = Codec_Write( AD1937_ADDRESS, reg, data);
    return err;
}


unsigned char I2CWrite_Codec_AIC3204( unsigned char reg,unsigned char data )
{
    unsigned char err;
    err = Codec_Write( CODEC_ADDRESS, reg, data);
    return err;
}



/******************************      AD1937        ************************************/
// OSC=12.288Mhz
/*


void ALL_POWER_ON(void)
{
    I2CWrite_Codec(DAC_Control0,0x00);  //
    I2CWrite_Codec(ADC_Control0,0x00);  //
}


void ALL_POWER_OFF(void)
{
    I2CWrite_Codec(DAC_Control0,0x01);  //
    I2CWrite_Codec(ADC_Control0,0x01);  //
}


unsigned char Codec_DAC_Attenuation( unsigned char DAC_NAME, unsigned int x10gain )
{
    
    unsigned char err;
    float temp ;
    
    if( x10gain == 1000 ){        
        return 0 ;
    }
    if (x10gain > 945) {//0.375*252=94.5dB         
        err = CODEC_SETVOL_RANGE_ERR;
        APP_TRACE_INFO(("ERR: CODEC Gain Over Range!\r\n"));        
        return err;
    }
    temp = x10gain/(0.375*10); 
    err = I2CWrite_Codec(DAC_NAME,(unsigned char)temp);
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }
    APP_TRACE_DBG(("\r\nSet CODEC REG[%d]=0x%0X",DAC_NAME,(unsigned char)temp)); 
    
    return 0;

}

//unsigned char Codec_DAC_Attenuation( unsigned char DAC_NAME, float gain )
//{
//    
//    unsigned char err;
//    float temp ;
//      
//    if ( gain > 0 ) {         
//        err = 1;
//        return err;
//    }
//    
//    if(gain< -95.25 ) {
//        temp = 255 ;
//        
//    } else {
//        temp = -gain/0.375; 
//        
//    }
//    
//    err = I2CWrite_Codec(DAC_NAME,(unsigned char)temp);
//    
//    return err;
//    
//}

unsigned short SR_Support[] =         {
                                            //8000,
                                            16000,
                                            24000,
                                            32000,
                                            //44100,
                                            48000
                                            //96000
                                      };

unsigned char Check_SR_Support( unsigned int sample_rate )
{    
   unsigned int i;
   for( i = 0; i<(sizeof(SR_Support)/2); i++ ) {
       if( SR_Support[i] == sample_rate ) {
           return OS_ERR_NONE ; //find the SR
       }
   }   
   return CODEC_SR_NOT_SUPPORT_ERR;  //SR not support
    
}


// Main clock source = 12.288 MHz.   
unsigned char Codec_SetFCLK( unsigned int fclk )
{
    
    unsigned char err;
    unsigned char pll_reg;
    
    switch( fclk )   {
      
        case 8000:  // Need  12.288 / 2 MHz Osc 
            pll_reg = 0x9e;      
            return CODEC_SETFCLK_RANGE_ERR;
            break;            
        case 16000:            
            pll_reg = 0x9e;            
            break;
        case 24000:      
            pll_reg = 0x9c;            
            break;
        case 32000:            
            pll_reg = 0x9a;      
            break;
        case 44100: // Need 11.2896 MHz Osc 
            pll_reg = 0x98;  
            return CODEC_SETFCLK_RANGE_ERR;
            break;
        case 48000:             
            pll_reg = 0x98;            
            break;
        case 96000: // Need 12.288 * 2 MHz Osc       
            pll_reg = 0x98;             
            return CODEC_SETFCLK_RANGE_ERR;
            break;
        case 0 : //power down clock
            pll_reg = 0x01; 
            break;
        default:
            return CODEC_SETFCLK_RANGE_ERR;
            break;
            
    }
    
    err = I2CWrite_Codec( PLL_Control0, pll_reg );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }

    return err;
    
}


unsigned char Codec_SetMode( unsigned char mode )
{
    unsigned char err;
    unsigned char dac_reg0,dac_reg1;
    unsigned char adc_reg1,adc_reg2;
    
    switch( mode )  {
    
        case I2S_MODE:
                 dac_reg0 = 0x00;      //I2S
                 dac_reg1 = 0x70;      //2 channels
                 adc_reg1 = 0x03;      //I2S
                 adc_reg2 = 0x01;      //2 channels
                 break;
        case TDM_MODE:
                 dac_reg0 = 0x40;      //TDM
                 dac_reg1 = 0x74;      // 8 channels
                 adc_reg1 = 0x23;      //TDM
                 adc_reg2 = 0x21;      // 8 channels
                 break;
        case TDM16_MODE:
                 dac_reg0 = 0x40;      //TDM
                 dac_reg1 = 0x76;      // 16 channels
                 adc_reg1 = 0x23;      //TDM
                 adc_reg2 = 0x31;      // 16 channels
                 break;
        default:
                return CODEC_SETMODE_RANGE_ERR;
                break;
    } 
    
    err = I2CWrite_Codec( DAC_Control0, dac_reg0 );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }    
    err = I2CWrite_Codec( DAC_Control1, dac_reg1 );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }
    
    err = I2CWrite_Codec( ADC_Control1, adc_reg1 );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }    
    err = I2CWrite_Codec( ADC_Control2, adc_reg2 );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }

    return err;    
  
}


unsigned char Init_CODEC( unsigned int sample_rate )   
{ 
    unsigned char err;
    unsigned char i;
    static unsigned int sr_saved; 
    
    unsigned char reg_para[][2] = {
        
        { PLL_Control0,0x98 },
        { PLL_Control1,0x00 },  //
        { DAC_Control2,0x18 },  // 16bit    
        { DAC_Mute    ,0xF0 },  // DAC3.4 mute.
        { DAC1L_Volume,0x00 },  // no attenuation on SPK
        { DAC1R_Volume,0x00 },  // no attenuation on SPK
        { DAC2L_Volume,0x00 },  // no attenuation on Lout
        { DAC2R_Volume,0x00 },  // no attenuation on Lout
        { ADC_Control0,0x30 }  //Enable ADC
 //    { DAC3L_Volume,0x00 },  // not used on AB03
 //    { DAC3R_Volume,0x00 },  // not used on AB03 
        
    };
    
    if( sample_rate == sr_saved ) {
        return 0;
    } else {
        sr_saved = sample_rate ;
    }

    for( i = 0; i< sizeof(reg_para)>>1; i++ ) {
      err = I2CWrite_Codec(reg_para[i][0], reg_para[i][1]); 
      if( OS_ERR_NONE != err ) {
          err = CODEC_WR_REG_ERR;
          return err ;
      }
      
    }
    
    err = Codec_SetFCLK( sample_rate );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }    

    err = Codec_SetMode( TDM_MODE );   
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }     

    if( !( Get_Switches() & 0x01 ) ) {
        err = CODEC_LOUT_Small_Gain_En( true ); //attenuation enable
    }
    
    return err;
    
}



*/

unsigned char CODEC_LOUT_Small_Gain_En( bool small_gain )
{
     /*
    unsigned char err;
    unsigned char reg;
    if( small_gain ) {        
        reg = 64;    // 64*0.375=24dB attenuation on Lout     
        APP_TRACE_INFO(("Lout Gain 24dB attenuation: Enabled \r\n"));  
        
    } else {        
        reg = 0;    // 0dB attenuation on Lout        
        APP_TRACE_INFO(("Lout Gain 24dB attenuation: Disabled \r\n"));  
        
    }    
    err = I2CWrite_Codec( DAC2L_Volume, reg );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }    
    
    err = I2CWrite_Codec( DAC2R_Volume, reg );
    if( OS_ERR_NONE != err ) {
        err = CODEC_WR_REG_ERR;
        return err ;
    }

    return err; 
    */
     return CODEC_FUNC_NOT_SUPPORT;
    
}


unsigned char CODEC_Set_Volume( unsigned int vol_spk, unsigned int vol_lin )
{
    /*
    unsigned char err;
    unsigned char temp;
    
    temp = 0xF0;
    err = Codec_DAC_Attenuation(DAC1L_Volume, vol_spk );    
    if( OS_ERR_NONE != err ){
        return err;
    }
    err = Codec_DAC_Attenuation(DAC1R_Volume, vol_spk );
    if( OS_ERR_NONE != err ){
        return err;
    }
    
    err = Codec_DAC_Attenuation(DAC2L_Volume, vol_lin );
    if( OS_ERR_NONE != err ){
        return err;
    }   
    err = Codec_DAC_Attenuation(DAC2R_Volume, vol_lin );
    if( OS_ERR_NONE != err ){
        return err;
    }
     
    if( vol_spk == SET_VOLUME_MUTE ) {
        temp += (3<<0);
    }
    if( vol_lin == SET_VOLUME_MUTE ) {
        temp += (3<<2);
    }
    err = I2CWrite_Codec(DAC_Mute,temp);
    
    return err;
    */
    return 0;
    
}




/*****************************************************************************/

unsigned short CODEC_PARA_TABLE[][14][7] = {
    { //mode 0
          //I2S format
          //BCLK = 16 * 2 * FCLK = 32 * FCLK
	  //parameter for MCLK = 24.576MHz 
          {48000, 44100, 32000, 24000, 22050, 16000, 8000 }, //SR	      
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x83 }, //  REG_NDAC    =
	  {0x82,  0x84,  0x82,  0x84,  0x84,  0x84,  0x81 }, //  REG_MDAC    = --R12
	  {0x80,  0x40,  0x80,  0x80,  0x80,  0x80,  0x00 }, //  REG_DOSR    = --R13-14
	  {0x84,  0x82,  0x84,  0x84,  0x84,  0x84,  0xA0 }, //  REG_BCLK_DIV=  --R30   
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x86 }, //  REG_NADC    = 
	  {0x84,  0x82,  0x84,  0x84,  0x82,  0x84,  0x84 }, //  REG_MADC    = 
	  {0x00,  0x80,  0x40,  0x80,  0x00,  0x80,  0x80 }, //REG_AOSR    =  --R20
	  //{3.072M, 2.8224M, 2.048M, 3.072M, 2.8224M, 2.048M, 1.024M},//     --PDMCLK   :       
	  {0x00,  0x03,  0x00,  0x00,  0x03,  0x00,  0x00 }, //CLK_MUX     =  --Select CODEC_CLKIN
	  {0,     1,     0,     0,     1,    0,     0}, //PLL_EN      = 
	  {0, 	  1,     0,     0,     1,    0,     0}, //PLL_R       = 
	  {0,     2,     0,     0,     2,    0,     0}, //PLL_P       = 
	  {0,     7,     0,     0,     7,    0,     0}, //PLL_J       = 
	  {0,     3500,  0,     0,     3500, 0,     0}  //PLL_D       = 
             
    
    },     

    { //mode 1
          //TDM16 format
          //BCLK = 16 * 8 * FCLK = 128 * FCLK
	  //parameter for MCLK = 24.576MHz 
          {48000, 44100, 32000, 24000, 22050, 16000, 8000 }, //SR	      
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x83 }, //  REG_NDAC    =
	  {0x82,  0x84,  0x82,  0x84,  0x84,  0x84,  0x81 }, //  REG_MDAC    = --R12
	  {0x80,  0x40,  0x80,  0x80,  0x80,  0x80,  0x00 }, //  REG_DOSR    = --R13-14
	  {0x82 , 0x82,  0x82,  0x84,  0x84,  0x84,  0x88 }, //  REG_BCLK_DIV=  --R30   
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x86 }, //  REG_NADC    = 
	  {0x84,  0x82,  0x84 , 0x84,  0x82,  0x84,  0x84 }, //  REG_MADC    = 
	  {0x00,  0x80,  0x40,  0x80,  0x00,  0x80,  0x80 }, //REG_AOSR    =  --R20
	  //{3.072M, 2.8224M, 2.048M, 3.072M, 2.8224M, 2.048M, 1.024M},//     --PDMCLK   :       
	  {0x00,  0x03,  0x00,  0x00,  0x03,  0x00,  0x00 }, //CLK_MUX     =  --Select CODEC_CLKIN
	  {0,     1,     0,     0,     1,    0,     0}, //PLL_EN      = 
	  {0, 	  1,     0,     0,     1,    0,     0}, //PLL_R       = 
	  {0,     2,     0,     0,     2,    0,     0}, //PLL_P       = 
	  {0,     7,     0,     0,     7,    0,     0}, //PLL_J       = 
	  {0,     3500,  0,     0,     3500, 0,     0}  //PLL_D       = 
             
    
    },
    
    { //mode 2
          //TDM32 format
          //BCLK = 32 * 8 * FCLK = 256 * FCLK
	  //parameter for MCLK = 24.576MHz 
          {48000, 44100, 32000, 24000, 22050, 16000, 8000 }, //SR	      
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x83 }, //  REG_NDAC    =
	  {0x82,  0x84,  0x82,  0x84,  0x84,  0x84,  0x81 }, //  REG_MDAC    = --R12
	  {0x80,  0x40,  0x80,  0x80,  0x80,  0x80,  0x00 }, //  REG_DOSR    = --R13-14
	  {0x81,  0x81,  0x81,  0x82,  0x82,  0x82,  0x84 }, //  REG_BCLK_DIV=  --R30   
	  {0x82,  0x88,  0x83,  0x82,  0x88,  0x83,  0x86 }, //  REG_NADC    = 
	  {0x84,  0x82,  0x84,  0x84,  0x82,  0x84,  0x84 }, //  REG_MADC    = 
	  {0x00,  0x80,  0x40,  0x80,  0x00,  0x80,  0x80 }, //REG_AOSR    =  --R20
	  //{3.072M, 2.8224M, 2.048M, 3.072M, 2.8224M, 2.048M, 1.024M},//     --PDMCLK   :       
	  {0x00,  0x03,  0x00,  0x00,  0x03,  0x00,  0x00 }, //CLK_MUX     =  --Select CODEC_CLKIN
	  {0,     1,     0,     0,     1,    0,     0}, //PLL_EN      = 
	  {0, 	  1,     0,     0,     1,    0,     0}, //PLL_R       = 
	  {0,     2,     0,     0,     2,    0,     0}, //PLL_P       = 
	  {0,     7,     0,     0,     7,    0,     0}, //PLL_J       = 
	  {0,     3500,  0,     0,     3500, 0,     0}  //PLL_D       = 
             
    
    }    
    
};

unsigned short BCLK_SOURCE[] = { 
          1, //I2S format
          0, //TDM16 format
          0  //TDM32 format
};
          


unsigned char config_aic3204[][2] = {
    
       
		      0,0x00, //page0 

		      //Software reset codec
		      1,0X01, 
		      
//		      //SET PLL == MCLK*R*J.D/P   mclk == 12.288MHz;
//		      4,CLK_MUX[SR_Index], 
//		      5,PLL_EN[SR_Index]*128 +PLL_P[SR_Index]*16 + PLL_R[SR_Index],
//		      6,PLL_J[SR_Index],
//		      7,math.floor( PLL_D[SR_Index]/256),
//		      8,PLL_D[SR_Index]%256,		      
//		   
//		      //Set DAC_fs == PLL/NDAC*MDAC*DOSR		  
//		      11,REG_NDAC[SR_Index],  //NDAC=3    
//		      12,REG_MDAC[SR_Index],  //MDAC=4
//		      //DOSR=128
//		      13,math.floor(REG_DOSR[SR_Index]/256), 
//		      14,REG_DOSR[SR_Index]%256,  
//		      
//		      //Set ADC_fs == PLL/NADC*MADC*AOSR   SET PDMCLK=ADC_MOD_CLK = 2.048MHz
//		      18,REG_NADC[SR_Index],  //NADC=3
//		      19,REG_MADC[SR_Index],  //MADC=4
//		      //AOSR=128
//		      20,REG_AOSR[SR_Index],  
//		      //if master mode,reg20,reg30 is needed.
//
//		      //BDIV_CLKIN Multiplexer Control			      	
//		      29,BCLK_SOURCE,           // ADC2DAC_ROUTED is not rounted  ; 
//		      //29,0X10+BCLK_SOURCE,    // ADC2DAC_ROUTED	
//		      30,REG_BCLK_DIV[SR_Index], //0X84,  //bclk=bdiv_clkin/4


		      //SET interface mode(I2S,PCM,Left,right)
		      27,0X0c,   //I2S mode,16bit,master
		      //27,0X00,   //I2S mode,16bit,Slave
		      //27,0X4c,   //DSP mode,16bit,master
		      //27,0Xcc,   //LJF mode,16bit,master
		      //27,0X8c,   //RJF mode,16bit,master
//		      27, HS,
//		      //Data offset
		      28,0X00,    //data offset == 0'bclk for I2S Mode, there have a cyale delay in I2S mode itself
//		      //28,0X01,    //data offset == 1'bclk for DSP Mode
		      32,0X00, 
		      33,0X4d, 
		      34,0X20, 
		      53,0X02,  	//Dout is pin5
		      54,0X02,  	//pin4 is i2s data input   	 	
		         
			  //-set DAC channels
		      63,0xE8, //DAC Channel Setup :  0xD4: L2L, R2R; 0xE8: L2R, R2L
		      64,0X00, // 	 
		      65,( 0X100+2*( -8 ) )%0x100, //DAC Volume L set -8dB  : [-63.5,+24] @ 0.5dB
		      66,( 0X100+2*( -8 ) )%0x100, //DAC Volume R set -8dB  : [-63.5,+24] @ 0.5dB
		      
		      //-set dmic data pin setting
		      55,0X0e,   // Set MISO as PDM CLK ouput pin
		      56,0X02,   // SCLK pin is enabled		
		      
		      81,0xD0,   // enable ADC and set SCLK as PDM DATA input pin//////-	    
		      //Dmic clock output(=adc_mod_clk), PDM CLK = ADC_MOD_CLK	        
		      82,0X00,   //ADC Fine gain adjust, 0dB, unmute
		      83,2*( 0 ),   //ADC Volume L set 0dB  : [-12,+20] @ 0.5dB   D?����0dB��?����??2?���?a??�����?����???����3
		      84,2*( 0 ),   //ADC Volume R set 0dB  : [-12,+20] @ 0.5dB   D?����0dB��?����??2?���?a??�����?����???����3


		      0,0X01,  //page1//////////////////////////
//		          
//		      //-set power
		      1, 0x08,  //disconnect AVDD and DVDD		 
		      2, 0X01,  //enable Master Analog Power Control		
		      3, 0X00,  //Set the DAC L PTM mode to PTM_P3/4
		      4, 0X00,  //Set the DAC R PTM mode to PTM_P3/4		      
		      9, 0XFF,  //All HPOUT,LOUT and Mixer Amplifier are Power up  
		      //9, 0x3C,
		      10,0X00,  //Set the Input Common Mode to 0.9V and Output Common Modefor Headphone to Input Common Mode
	              20,0X00,  //headphone driver startup

		      //-set route settings
		      //CODEC LO to FL124 LIN, single ended
	              12,0X08, //HPL route on
		      13,0X08, //HPR route on
		      14,0X08, //LOL route on
		      15,0X08, //LOR route on
		      
		      //Analog input mixer settings
		      52,0X40, // IN1L to L_MICPGA 
		      54,0X40, // CM1L to L_MICPGA 
		      55,0X40, // IN1R to R_MICPGA 
		      57,0X40, // CM1R to R_MICPGA 	      
		         
		      //-set DAC output gains
		      16,( 0X40+( 12 ) )% 0x50 ,  //HPL +12db gain :  [-6,+29] @ 1dB
		      17,( 0X40+( 12 ) )% 0x50 ,  //HPR +12db gain :  [-6,+29] @ 1dB
		      18,( 0X40+( -6 ) )% 0x50 ,  //LOL -6db gain   :  [-6,+29] @ 1dB
		      19,( 0X40+( -6 ) )% 0x50 ,  //LOR -6db gain   :  [-6,+29] @ 1dB
		      		       
		      //-set MIC PGA Gain
		      59,0X00,  //L_MICPGA 0db gain
		      60,0X00,  //R_MICPGA 0db gain	

		     	    
};




#define CFG_PARA_NUM  14

unsigned char Init_CODEC( unsigned int sr, unsigned char sample_length ) 
{
    unsigned char err;
    unsigned char mode;
    unsigned char reg_data[CFG_PARA_NUM];
    unsigned char sr_index = 255; 
    unsigned char reg_index[CFG_PARA_NUM] = {
        4, 5, 6, 7, 8, 11, 12, 13, 14, 18, 19, 20, 29, 30
    };

    for( unsigned char i = 0; i<sizeof(CODEC_PARA_TABLE[0][0])>>1; i++ ) {
        if(CODEC_PARA_TABLE[0][0][i] == sr ) {
            sr_index = i;
            break;
        }
    }
    if(sr_index == 255) {
        return CODEC_SR_NOT_SUPPORT_ERR;
    }
    
    if( sample_length == 16 ) {
        mode = 1;
        //mode = 0; //for iM401
    } else {//if(sample_length == 32 ) {
        mode = 2;
    } //else {
        //return CODEC_SR_LEN_NOT_SUPPORT_ERR;
    //}
    
    reg_data[0] = CODEC_PARA_TABLE[mode][8][sr_index];
    reg_data[1] = CODEC_PARA_TABLE[mode][10][sr_index]*128+CODEC_PARA_TABLE[mode][12][sr_index]*16+ CODEC_PARA_TABLE[mode][11][sr_index];
    reg_data[2] = CODEC_PARA_TABLE[mode][13][sr_index];
    reg_data[3] = CODEC_PARA_TABLE[mode][14][sr_index]>>8;
    reg_data[4] = CODEC_PARA_TABLE[mode][14][sr_index];    
    reg_data[5] = CODEC_PARA_TABLE[mode][1][sr_index];
    reg_data[6] = CODEC_PARA_TABLE[mode][2][sr_index];    
    reg_data[7] = CODEC_PARA_TABLE[mode][3][sr_index]>>8;
    reg_data[8] = CODEC_PARA_TABLE[mode][3][sr_index];    
    reg_data[9] = CODEC_PARA_TABLE[mode][5][sr_index];
    reg_data[10] = CODEC_PARA_TABLE[mode][6][sr_index];
    reg_data[11] = CODEC_PARA_TABLE[mode][7][sr_index]; 
    reg_data[12] = BCLK_SOURCE[mode];
    reg_data[13] = CODEC_PARA_TABLE[mode][4][sr_index];
        
    for( unsigned char i = 0 ; i< sizeof(config_aic3204)>>1 ; i++ ) {    
        err = I2CWrite_Codec_AIC3204(config_aic3204[i][0],config_aic3204[i][1]);
        if( OS_ERR_NONE != err ){
          return err;
        }
    }
    
    err = I2CWrite_Codec_AIC3204(0,0);
    if( OS_ERR_NONE != err ){
       return err;
    }
    
    for( unsigned char i = 0 ; i< CFG_PARA_NUM ; i++ ) {    
        err = I2CWrite_Codec_AIC3204(reg_index[i],reg_data[i]);
        if( OS_ERR_NONE != err ){
          return err;
        }
    }
    
    return err;    
    
}


unsigned char Set_AIC3204_DSP_Offset( unsigned char slot_index ) 
{
    
    unsigned char err;
    
    if( slot_index >6 ) { //slot_index is for line in channels
        return 0x99;
    }      
    err = I2CWrite_Codec_AIC3204( 28, slot_index << 5);
    
    return err;    
    
}


unsigned char Check_SR_Support( unsigned int sample_rate )
{    
   unsigned int i;
   for( i = 0; i<sizeof(CODEC_PARA_TABLE[0][0])>>1; i++ ) {
       if( CODEC_PARA_TABLE[0][0][i] == sample_rate ) {
           return OS_ERR_NONE ; //find the SR
       }
   }   
   return CODEC_SR_NOT_SUPPORT_ERR;  //SR not support
    
}