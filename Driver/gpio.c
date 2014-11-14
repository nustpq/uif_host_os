#include <includes.h>

GPIOPIN   GpioPin  ;
GPIODIR   GpioDir  ;
GPIOPORT  GpioPort ;

unsigned int MonitCount = 0;
unsigned char setdir    = 0;


//inner use
static Pin pinsGpios[]  = {          
    
    GPIO_0,     GPIO_1,      GPIO_2,     GPIO_3,     GPIO_4,     GPIO_5,     GPIO_6,     GPIO_7,// 0 ~ 7
    LED1,       LED2, // 8 ~ 9   
    GPIO_0_LED, GPIO_1_LED,  GPIO_2_LED, GPIO_3_LED, GPIO_4_LED, GPIO_5_LED, GPIO_6_LED, GPIO_7_LED, // 10~17    
    GPIO_PDMIN_CLK_DIR,  GPIO_MATER_SLAVE_CTR, GPIO_CODEC_RESET, GPIO_AUDIO_RST, GPIO_FM_RST, // 18 ~ 22   
    BUZZER    // 23
                                  
};



//switch pins
static const Pin pinsSwitches[] = {   SW0,      SW1, 
                                        MODE_SW0, MODE_SW1, MODE_SW2, MODE_SW3 };


static const unsigned int numGpios = PIO_LISTSIZE(pinsGpios);




void GPIO_Init(void)
{  
  
    PIO_Configure( pinsSwitches,  PIO_LISTSIZE(pinsSwitches) );
    PIO_Configure( pinsGpios,     PIO_LISTSIZE(pinsGpios)     );          
    //PIO_InitializeInterrupts( AT91C_AIC_PRIOR_LOWEST );     
    //LED_Configure(LED_DS1);
    //LED_Configure(LED_DS2);
    
}



void GPIODIR_FLOAT( unsigned int pin  ) // 
{  
     unsigned int i; 
     
     for(i=0; i<numGpios; i++)
     {       
       if( pin>>i & 0x01 )
       {             
           pinsGpios[i].attribute  = PIO_DEFAULT ; 
           pinsGpios[i].type       = PIO_INPUT   ;            
           PIO_Configure(&pinsGpios[i], 1);
       }
       
     }

   
}

////ranfunc for a faster execution 
//void GPIOPIN_Set(unsigned int pin , unsigned int dat)
//{  
//    unsigned int i; 
//    
//    for( i=0; i < numGpios; i++ ) {       
//        if( (pin >> i) & 0x01 ) { 
//            pinsGpios[i].attribute  = PIO_PULLUP ;                   
//            if( (dat >> i) & 0x01 ) {
//              pinsGpios[i].type    = PIO_OUTPUT_1   ;  
//              
//            } else {
//              pinsGpios[i].type    = PIO_OUTPUT_0   ; 
//              
//            }
//            PIO_Configure(&pinsGpios[i], 1);
//            
//       }
//       
//     } 
//    
//}


//ranfunc for a faster execution 
void GPIOPIN_Set(unsigned int pin , unsigned int dat)
{  




    pinsGpios[pin].attribute  = PIO_PULLUP ;                   
    if( dat  & 0x01 ) {
         PIO_Set(&pinsGpios[pin]);
              
    } else {
        PIO_Clear(&pinsGpios[pin]);
              
    }
         

    
}

//// additional time delay :  +10us
//// so, the critical time delay is 11us
void  __ramfunc GPIOPIN_Set_Session( unsigned int pin , unsigned int dat )
{    
    unsigned int i; 
    
    for( i=0; i < 28; i++ ) {  //here 28 is used instead of numGpios for speed up !
      
        if( pin & 0x01<<i ) { 
                            
            if( dat  & 0x01<<i ) {
                //PIO_Set( &pinsGpios[i]);
                pinsGpios[i].pio->PIO_SODR = pinsGpios[i].mask;
            } else {
                //PIO_Clear( &pinsGpios[i]);
                pinsGpios[i].pio->PIO_CODR = pinsGpios[i].mask;              
            }              
        }         
     }     
}


typedef struct __MONITCTR 
{
    unsigned char DataType ;
    unsigned short  DataNum ;
    
}MONITCTR ;


unsigned int GPIOPIN_Read(void)
{
  /*pq
    GpioPin.portStt.porta = GPIO_ReadPin(PA) ;
    GpioPin.portStt.portc = GPIO_ReadPin(PC) ;
    GpioPin.portStt.portg = GPIO_ReadPin(PG) ;
    GpioPin.portStt.portj = GPIO_ReadPin(PJ) ;
    
    return(GpioPin.pinStt & PINMASK) ;
  */
  return 0;
}


void  RecordGpio(
               unsigned char pTime,  /*读取数据的时间间隔,单位为uS*/
               unsigned short  dTime,  /*记录数据时间长度,单位为mS*/
               void *p               /*存放数据位置*/
               )
{
  
    /*pq
    MONITCTR *pMonitCtr = (MONITCTR *)DataBufCtr.pBufTop ;
    unsigned char *pDataSt ;
    unsigned short tdelay ;
    DataBufCtr.pBufTop += 3 ;
    pMonitCtr->DataType = DATA_TYPE_MONIT ;

    pDataSt  = DataBufCtr.pBufTop ;
    MonitCount = 0;
    pinb = 0x76;
    
    TIMER_Open_8bit(0,T_INT_EN|T_PRS_8, pTime-2) ;

    while(dTime)
    {
        tdelay = ((dTime>4000)? 4000:dTime) ;
        dTime -= tdelay ;
        CtrFlage.Time3Over = 0 ;
        
        TIMER_Open_16bit(3,T_INT_EN|T_PRS_1024, tdelay) ;
        
        while(!CtrFlage.Time3Over) ;
    }

    TCCR0B &= 0xf8 ;      // 关闭定时器0
    OCR0A = 0;            
    OCR0B = 0;    
    *(unsigned int *)DataBufCtr.pBufTop = MonitCount ;
    DataBufCtr.pBufTop += 4 ;
    *(DataBufCtr.pBufTop++) = PINB ;
    *(DataBufCtr.pBufTop++) = PINC ;
    *(DataBufCtr.pBufTop++) = PIND ;
      DataBufCtr.pBufTop++ ;
    pMonitCtr->DataNum = (DataBufCtr.pBufTop - pDataSt)/8 ;
  
    */
}






void Ruler_Power_Switch( unsigned char onoff )
{

//    pinsGpios[5].type = (onoff == 0) ? PIO_OUTPUT_0 : PIO_OUTPUT_1 ;
//    PIO_Configure(&pinsGpios[5], 1);   
    
}




//Ruler MCU selector
void UART1_Mixer( unsigned char index )
{      
//    unsigned char i; 
//    //OSTimeDly(1000);
//    if( index<= 3) {      
//        for( i=0; i<=1; i++) {        
//            pinsGpios[i].type = (index & (1<<i) ) == 0 ? PIO_OUTPUT_0 : PIO_OUTPUT_1 ;
//            PIO_Configure(&pinsGpios[i], 1);   
//      
//        }      
//    
//    }   
//    //OSTimeDly(50);
  
}


//Audio MCU selector
void UART2_Mixer( unsigned char index )
{  
    
#ifdef BOARD_TYPE_AB01     
    unsigned char i; 
  
    if( index<= 3) {      
        for( i=2; i<=3; i++) {        
            pinsGpios[i].type = (index & (1<<(i-2)) ) == 0 ? PIO_OUTPUT_0 : PIO_OUTPUT_1 ;
            PIO_Configure(&pinsGpios[i], 1);   
      
        }      
    
    }   
#endif  
}


unsigned int Get_Switches( void )
{
      
    unsigned char i     =   0 ;
    unsigned int  value =   0 ;
  
    for( i=0; i<PIO_LISTSIZE( pinsSwitches ); i++ ) {      
        value <<= 1;
        value +=PIO_Get( &pinsSwitches[i] );
    }
    
    return value;
  
}



unsigned int Get_Port_Detect( void )
{
    
//    unsigned char i     =   0 ;
//    unsigned int  value =   0 ;   
//
//    for( i=9; i<=12; i++ ) {      
//        value <<= 1;
//        value +=PIO_Get( &pinsGpios[i] );
//    }
//           
//    return value;   

}



//note: turn off and on to update a ruler firmware, hwo about other ruler connections , issue ???
void Ruler_PowerOnOff( unsigned char switches ) 
{
    
//    if( switches == 0 ) { //power off
//        PIO_Clear(&pinsGpios[5]);  
//    } else { //power on
//        PIO_Set(&pinsGpios[5]);  
//    }
     
}


void Pin_Reset_FM36( void )
{

    PIO_Clear(&pinsGpios[22]);
    OSTimeDly(20) ;
    PIO_Set(&pinsGpios[22]);
    OSTimeDly(50) ;
     
}


void Pin_Reset_Audio_MCU( void )
{

    PIO_Clear(&pinsGpios[7]);
    OSTimeDly(21) ;
    PIO_Set(&pinsGpios[7]);
    OSTimeDly(21) ;
     
}

void Enable_FPGA( void )
{
    
    //PIO_Set(&pinsGpios[4]);   
     
}


void Disable_FPGA( void )
{
    
    //PIO_Clear(&pinsGpios[4]);    
     
}



//Note: This routine do NOT support reentrance
//SPI simulation for FPGA control timing requirement
//31 bits control 31 mics on 4 rulers, 1- enable, 0- disable
//channels[7..0]   : ruler0_mic[7..0]
//channels[15..8]  : ruler1_mic[7..0]
//channels[23..16] : ruler2_mic[7..0]
//channels[31..24] : ruler3_mic[7..0]
void Init_FPGA( unsigned int channels )
{
   
    unsigned int i ;
    APP_TRACE_DBG(("Init FPGA...[0x%0X] \r\n",channels));
    PIO_Set(&pinsGpios[20]); //cs 
    PIO_Set(&pinsGpios[21]); //data 
    PIO_Set(&pinsGpios[22]); //clock
    for ( i = 0; i < 32; i++ ) {        
       PIO_Clear(&pinsGpios[20]); //cs, delay compensation
       PIO_Clear(&pinsGpios[22]); //clock       
       if( (channels<<i) & 0x80000000 ) {
           PIO_Set(&pinsGpios[21]); //data 
       } else {
           PIO_Clear(&pinsGpios[21]); //data 
       }
       PIO_Set(&pinsGpios[22]); //clock
    }    
    PIO_Set(&pinsGpios[20]); //cs 
    
}

