/*****************************************************************************************/
/* AT_PC_com.h: Functions prototypes for communication between ATMEL processor and PC    */
/* by Skleki                                                                             */
/*****************************************************************************************/
#include "konstante.h"

extern unsigned char idata data_buf[MaxTxMsgLen];
extern unsigned long idata PCAliveCounter;

extern void InitCOM (void);
extern void SendDataToPC (unsigned char databytes[], unsigned int DataLength);
extern bit MsgFromPCParser(void);

