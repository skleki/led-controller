// konstante.h

#define XTAL 18432000L

#define TX_BUF_SIZE 250
#define RX_BUF_SIZE 250 
#define RX_BUF_TEMP_SIZE 50
#define MaxRxMsgLen 20
#define MaxTxMsgLen 20

#define EWDT_RESET_INTERVAL 250				// u milisekundama
#define TASTER_SETTLE_TIME 2


// CasinoLED V1.0 settings
#define EWDT_RESET_LINE_SIG_LED P3_3
#define RED_LINE P1_4
#define GREEN_LINE P1_5
#define BLUE_LINE P1_3
#define COLOR_LINES P1
#define TASTERI P2
