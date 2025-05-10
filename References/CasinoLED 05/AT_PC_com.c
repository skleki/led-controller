/******************************************************************************/
/* AT_PC_com.c: Functions for communication between AT processor and PC       */
/* by Skleki                                                                  */
/******************************************************************************/

//#include <reg8253.h>
#include <at89c51xd2.H>
#include <intrins.h>
#include <stdlib.h>
#include "at_pc_com.h"
#include "uart.h"
#include "wdt.h"
#include "konstante.h"


// Debug data
//unsigned char xdata NumOfMsgsReceived = 0;
//char xdata LCDmes[12];

unsigned char idata data_buf[MaxTxMsgLen];
unsigned char xdata mes[MaxRxMsgLen];

// DEKLERACIJE FUNKCIJA
bit MsgFromPCParser();
void MsgFromPCAnalyser(void);
unsigned char MsgValid(unsigned char* mes, unsigned char MsgLen);


void InitCOM (void)
{
//	unsigned char xdata i = 0xA0;
	init_com ();
}

void SendDataToPC (unsigned char databytes[], unsigned int DataLength)
{
	unsigned short xdata i;	
	unsigned char xdata cs;

	putchar (0xF0);			 	// Message footer
	putchar (DataLength + 4);	// 4 bajta je za header i footer poruke ka PC-ju

	cs = DataLength + 4;
	for (i = 0; i < DataLength; i++)
	{
		putchar (databytes[i]);
		cs = cs ^ databytes[i];
	}

	putchar (cs);
	putchar (0xFE);
}

unsigned char MsgValid(unsigned char* mes, unsigned char MsgLen)
{
  unsigned char xdata cs;
  int i;

  cs = 0;
  for (i = 1; i <= (MsgLen - 3); i++)
    cs  = cs ^ mes[i];

  if (cs == mes[MsgLen - 2])
    return 1;
  else
    return 0;
}





// Message Analyser
void MsgFromPCAnalyser (void)
{
	switch (mes[2])
	{
//		case 0x10 :
//			PCAliveCounter = 0;
//			break;
	}	// za switch (mes[2])
	return;
}






bit MsgFromPCParser()
{
	unsigned int xdata MsgLen;
	unsigned int xdata StartP, MaxLen, i, tmpind;

	bit UART_Parse_RxFret;

	unsigned int xdata CurrentMsgLen;
	unsigned int xdata CurrentMaxMsgLen;
	bit MsgParsedOK;

	CurrentMsgLen = 0;
	CurrentMaxMsgLen = 0;
	
//	Disable_UART;

	MsgParsedOK = 0;

	while ((RxBuf[RxBufOutP] != 0xF0) && (RxBufFree < RX_BUF_SIZE))
	{
		RxBufOutP++;
		RxBufFree++;

		if (RxBufOutP == RX_BUF_SIZE)
			RxBufOutP = 0;
	}

// *********************************************************************
	  if ((RX_BUF_SIZE - RxBufFree) > 4)   // Minimalna poruka je velicine 5 bajtova
	  {
	    if (RxBufOutP < RxBufInP)  // Slucaj kada bafer nije poceo da se puni iz pocetka
	    { 
	        MaxLen = RxBufInP - RxBufOutP;
	        StartP = RxBufOutP;
	        MsgLen = RxBuf[RxBufOutP + 1];
	
			CurrentMsgLen = MsgLen;
			CurrentMaxMsgLen = MaxLen;
	        // ovde ubaci proveru sta se radi ako stigne losa poruka gde iza hedera
	        // i tipa poruke ide broj koji pokazuje mnogo veliku vrednost.
	        // Stavi ogranicenje na velicinu poruke
	        if ((MsgLen > MaxRxMsgLen) || (MsgLen == 0))
	        	RxBuf[RxBufOutP] = 0; // U narednom pozivu PARSERA ce se ovaj pocetak prebrisati i nema frke
	        else if (MsgLen <= MaxLen)
	        {
	          if (RxBuf[StartP + MsgLen - 1] == 0xFE) // Detektovan validan kraj poruke
	          {
	              // *********************************************
	              for(i = 0; i < MsgLen; i++)
	                mes[i] = RxBuf[StartP + i];
	              if (MsgValid(mes, MsgLen))
	              {
	                  // Pozovi parser za primljene poruke
	                  RxBufOutP = RxBufOutP + MsgLen;
	                  RxBufFree = RxBufFree + MsgLen;
                    MsgParsedOK = 1;
	              }
	              else // Oslobodi bafer
	              {
	                  RxBufOutP = RxBufOutP + MsgLen;
	                  RxBufFree = RxBufFree + MsgLen;
				  }
	          }
	          else
	          	RxBuf[StartP] = 0; // U narednom pozivu PARSERA ce se ovaj pocetak prebrisati i nema frke
	        }
	    } // Za slucaj kada bafer nije poceo da upoisuje od pocetka
	    else  // Slucaj kada je bafer poceo da se puni iz pocetka
	          // RxBufOutP > RxBufInP
	    {
	        MaxLen = RX_BUF_SIZE + RxBufInP - RxBufOutP;
	        StartP = RxBufOutP;
	        tmpind = StartP + 1;
	        tmpind = tmpind % RX_BUF_SIZE;
	        MsgLen = RxBuf[tmpind];
	
			CurrentMsgLen = MsgLen;
			CurrentMaxMsgLen = MaxLen;

	        if ((MsgLen > MaxRxMsgLen) || (MsgLen == 0))
	          RxBuf[StartP] = 0;
	        else if (MsgLen <= MaxLen)
	        {
	            tmpind = StartP + MsgLen - 1;
	            tmpind = tmpind % RX_BUF_SIZE;
	            if (RxBuf[tmpind] == 0xFE)
	            {
		            for(i = 0; i < MsgLen; i++)
	                {
	                    tmpind = StartP + i;
	                    tmpind = tmpind % RX_BUF_SIZE;
	                    mes[i] = RxBuf[tmpind];
	                }
	                if (MsgValid(mes, MsgLen))
	                {
	                    RxBufOutP = RxBufOutP + MsgLen;
	                    RxBufOutP = RxBufOutP % RX_BUF_SIZE;
	                    RxBufFree = RxBufFree + MsgLen;
                      MsgParsedOK = 1;
	                }
	                else
	                {
	                    RxBufOutP = RxBufOutP + MsgLen;
	                    RxBufOutP = RxBufOutP % RX_BUF_SIZE;
	                    RxBufFree = RxBufFree + MsgLen;
	                }
	            }
				else
	              RxBuf[StartP] = 0;
	        }
	    }
	  }
	
	  if (((RX_BUF_SIZE - RxBufFree) < 5) || (CurrentMsgLen > CurrentMaxMsgLen))
	    UART_Parse_RxFret = 0;
	  else
	  	UART_Parse_RxFret = 1;
	  	

// *********************************************************************

//	Enable_UART;

	if (MsgParsedOK)
		MsgFromPCAnalyser ();

	return UART_Parse_RxFret;
}

