// UART.h
#include "konstante.h"

#define Enable_UART REN = 1		// Enable Serial Port Interrupt	
#define Disable_UART REN = 0		// Disable Serial Port Interrupt	

extern unsigned char xdata RxBufTemp[RX_BUF_TEMP_SIZE];
extern unsigned int xdata RxBufTempInP;
extern bit RxBufTempNotEmpty;

extern unsigned char xdata TxBuf[TX_BUF_SIZE];
extern unsigned char xdata RxBuf[RX_BUF_SIZE];
extern unsigned int xdata TxBufInP;
extern unsigned int xdata TxBufOutP;
extern unsigned int xdata TxBufFree;
extern unsigned int xdata RxBufInP;
extern unsigned int xdata RxBufOutP;
extern unsigned int xdata RxBufFree;

extern void init_com (void);
extern char putchar (char c);
extern bit RxParseFlag;
