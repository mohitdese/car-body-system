#include "mcu_avr_atmega128_api.h"
volatile char api_break=0;
/*********************************************************************/
/*********************************************************************/
/******************  UART1 (diagnostic serial port)   ****************/
/*********************************************************************/
/*********************************************************************/
// Implemented by LIN

/*********************************************************************/
/*********************************************************************/
/******************         SPI interface		      ****************/
/*********************************************************************/
/*********************************************************************/
void (*SPI_IT_Handler)(void)=NULL; //pointer to IT handler function - set in UART1_Init
SIGNAL(SIG_SPI) {SPI_IT_Handler();}


void SPI_Init(unsigned char cpol, unsigned char cpha,unsigned char lsbf, unsigned char speed)
{
	DDRB|=0x07; DDRB&=0xf7;  //MOSI SCK SS output, MISO input 
	SPCR = _BV(SPE)|_BV(MSTR) | (cpol<<CPOL) | (cpha<<CPHA) | (lsbf<<DORD);
	if(speed==3 || speed==4 || speed==7) SPCR|= _BV(SPR0);
	if(speed>=5) SPCR|= _BV(SPR1);		
	if(speed==1 || speed==3 || speed==5) SPSR|=_BV(SPI2X);		
}


void SPI_SlaveInit(void (*handler)(void)) //argument: interrupt handler function
{
	DDRB|=0x08; DDRB&=0xf8;   
	SPCR = _BV(SPE);
	if(handler) {SPI_IT_Handler=handler; SPCR |= _BV(SPIE);}
}

unsigned char SPI_byte(unsigned char data_write, unsigned char* data_read)
{
	SPDR=data_write;
//	printf("API > SPI sending\r\n");
	loop_until_flag_is_set(SPSR,SPIF);
	if(data_read) *data_read=SPDR;
//	printf("API > SPI done\r\n");
	return 0; //SPI_NOERROR; //TODO
}

/*********************************************************************/
/*********************************************************************/
/******************         I2C interface		      ****************/
/*********************************************************************/
/*********************************************************************/




void I2C_init (int fr)
{
	//Setting Bitrate
	TWSR=0;
	TWBR=((int)(F_CPU/1000L/fr)-16)/2;
	TWCR=_BV(TWEN);
}

unsigned char I2C_start(unsigned char addr, unsigned char rnw) //internal
{
	//IT table: 215., 219. p.
	addr<<=1;
	if(rnw) addr|=1;							//Set RW flag
	unsigned char state;
	int i;
	
	for(i=0; i<1000 && !api_break; ++i)		//try the starting more times
	{
		TWCR=_BV(TWINT)|_BV(TWSTA)|_BV(TWEN);		//Send START condition
		loop_until_flag_is_set(TWCR, TWINT);		//wait for TWINT = START transmitted
		state=TWSR & 0xf8;
		if(state!=0x08 && state!=0x10) continue;	//if no (repeated) START transmitted

		TWDR=addr;								//load address	
		TWCR=_BV(TWINT)|_BV(TWEN);					//Clear IT flag, start transmission
		loop_until_flag_is_set(TWCR, TWINT);			//wait for TWINT = addr transmitted, acked
		state=TWSR & 0xf8;
		if(    ( (!rnw) && (state==0x18)) ||				//if no SLA+W ACK in write mode and
			   (  rnw   && (state==0x40)))				//no SLA+R ACK in read mode
					return I2C_NOERROR;
	}
	return I2C_ERROR;

}

unsigned char I2C_start_write (unsigned char addr)
{
	return I2C_start(addr,0);
}

unsigned char I2C_write (unsigned char data)
{
	TWDR=data;
	TWCR=_BV(TWINT)|_BV(TWEN);					//Clear IT flag, start transmission
	loop_until_flag_is_set(TWCR, TWINT);		//wait for TWINT = data transmitted, acked
	if((TWSR & 0xf8)!=0x28) 
	{
//		printf("api> I2C write error!\r\n");
		return I2C_ERROR;	//if no data ACK received
	}
	return I2C_NOERROR;
}

unsigned char I2C_start_read (unsigned char addr)
{
	return I2C_start(addr,1);	
}

unsigned char I2C_read (unsigned char *data, unsigned char last_byte)
{
	if(last_byte) //No ACK
		TWCR = _BV(TWINT) | _BV(TWEN);
	else
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	loop_until_flag_is_set(TWCR, TWINT);			//wait for TWINT = data received
	if(api_break) return I2C_ERROR;	
	*data=TWDR;									//read data
	return I2C_NOERROR;
}

void I2C_stop (void)
{
	TWCR=_BV(TWINT)|_BV(TWSTO)|_BV(TWEN);		//Send STOP condition
}
