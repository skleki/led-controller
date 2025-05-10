// UART code file
//#include <reg8253.h>
#include <at89c51xd2.H>
#include "uart.h"
#include "konstante.h"

volatile unsigned char xdata RxBufTemp[RX_BUF_TEMP_SIZE];
volatile unsigned int xdata RxBufTempInP;
volatile bit RxBufTempNotEmpty;
volatile unsigned char xdata TxBuf[TX_BUF_SIZE];
unsigned char xdata RxBuf[RX_BUF_SIZE];
volatile unsigned int xdata TxBufInP;
volatile unsigned int xdata TxBufOutP;
volatile unsigned int xdata TxBufFree;
unsigned int xdata RxBufInP;
unsigned int xdata RxBufOutP;
unsigned int xdata RxBufFree;

bit COM_Tx_Disabled;
bit RxParseFlag;

// Podesavanje Timer1 da bude BAUD generator
void com_baudrate (unsigned baudrate)
{
	EA = 0;			// Disable All Interrupts                         
	COM_Tx_Disabled = 1;	// Transmit disabled bit

/*	// This uses Timer 1 as baud generator for 9600bps
	TR1 = 0;		// Stop Timer1
	ET1 = 0;		// Disable Timer1 Interrupt
	
	RCLK = 0;
	TCLK = 0;		// Selektovan je Timer1 za BaudRate Generator za UART
	
	PCON |= 0x80;
	TMOD = 0x21;
	TH1 = (unsigned char) (256 - (XTAL / (16L * 12L * baudrate)));
	TH0 = 250;

	ES=1;
	TR1 = 1;		// Start Timer1
					// Timer1 MUST NOT have enabled interrupt
*/

	// This uses internal baud rate generator for 115200bps
	baudrate = baudrate;	// dummy zbog kompajler warning
	BRL = 251;	// vidi tablisu iz excel fajla
	PCON |= (1 << 7);	// setting SMOD bit - double speed
	BDRCON = (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1);

	// This uses internal baud rate generator for 9600bps
//	baudrate = baudrate;	// dummy zbog kompajler warning
//	BRL = 251;	// vidi tablisu iz excel fajla
//	BDRCON = (1 << 4) | (1 << 3) | (1 << 2);


	EA = 1;			// Enable All Interrupts
}



void init_com (void)
{
	RxBufTempInP = 0;
  	RxBufTempNotEmpty = 0;
  
	RxBufFree = RX_BUF_SIZE;
	RxBufInP = 0;
	RxBufOutP = 0;
	TxBufFree = TX_BUF_SIZE;
	TxBufInP = 0;
	TxBufOutP = 0;
	RxParseFlag = 0;

	com_baudrate (9600);
	
	EA = 0;		// Disable All Interrupts
		
	SM0 = 0; 
	SM1 = 1;	// UART mode 1 selected (8-bit, 1 stop bit, no parity, baud rate variable)
	SM2 = 0;	// No multiprocessor communication

	REN = 1;	// Enable Receive
	ES = 1;		// Enable Serial Port Interrupt	
	
	EA = 1;		// Enable All Interrupts


}


// Funkcija koja upisuje karakter za slanje u OUTPUT bafer
char com_putchar (unsigned char c)
{
	if (!TxBufFree)	// If the buffer is not free, return an error value.
	  return (-1);
	
	EA = 0;                         					// Disable Interrupts
	TxBuf [TxBufInP++] = c;				// Add the data to the transmit buffer
	TxBufInP %= TX_BUF_SIZE;
	TxBufFree--;
	
	if (COM_Tx_Disabled)								// if transmitter is disabled 
	{
		COM_Tx_Disabled = 0;
		TI = 1;											// enable it 
	}
	
	EA = 1;                         					// Enable Interrupts
	
	return (0);
}


char putchar (char c)
{
	while (com_putchar (c) != 0);
	return (c);
}



/******************************************************************************/
/*       serial:  serial receiver / transmitter interrupt                     */
/******************************************************************************/
serial () interrupt 4 using 2    	// UART is interrupt 4
{     								/* use registerbank 2 for interrupt      */
	unsigned char c;

	if (RI)                         /* if receiver interrupt                 */
	{
		RI = 0;						/* reset interrupt flag                  */

		c = SBUF;					/* read character                        */
		if (RxBufTempInP < RX_BUF_TEMP_SIZE)
		{
			RxBufTemp[RxBufTempInP++] = c;
			RxBufTempNotEmpty = 1;
		}
	}


	if (TI)							/* if transmitter interrupt              */
	{
		TI = 0;						/* reset interrupt flag                  */

		if (TxBufFree < TX_BUF_SIZE)
		{
			SBUF = TxBuf [TxBufOutP++];
			if (TxBufOutP == TX_BUF_SIZE)
				TxBufOutP = 0;
			TxBufFree++;
		}
		else
			COM_Tx_Disabled = 1;
	}
}

