#include "CAN.h"
#include "macros.h"
#include "GPIO.h"
#include <stm32f303xe.h>

#define CAN_EN RCC->APB1ENR |= RCC_APB1ENR_CANEN
#define CAN_ABOM_EN CAN->MCR |= CAN_MCR_ABOM
#define CAN_AWUM_EN CAN->MCR |= CAN_MCR_AWUM
#define CAN_SET_MODE_INIT CAN->MCR |= CAN_MCR_INRQ 
#define CAN_SET_MODE_NORM CAN->MCR &= ~CAN_MCR_INRQ

#define CAN_RX_PIN PA11
#define CAN_TX_PIN PA12

/* CAN identifier type */
#define CAN_ID_STD            ((uint32_t)0x00000000)  /* Standard Id          */
#define CAN_ID_EXT            ((uint32_t)0x00000004)  /* Extended Id          */

/* CAN remote transmission request */
#define CAN_RTR_DATA          ((uint32_t)0x00000000)  /* Data frame           */
#define CAN_RTR_REMOTE        ((uint32_t)0x00000002)  /* Remote frame         */

// CAN: PA12 CTX, PA11 CRX

CAN_msg       CAN_RxMsg;      // CAN message for receiving                                
unsigned int  CAN_TxRdy;      // CAN HW ready to transmit a message
unsigned int  CAN_RxRdy;      // CAN HW received a message


void canStart (void)  {
  CAN_SET_MODE_NORM;  // normal operating mode, reset INRQ
  while (CAN->MSR & CAN_MCR_INRQ); // wait for CAN to start

}

typedef const struct
{
	uint8_t TS2;
	uint8_t TS1;
	uint8_t BRP;
} CAN_bit_timing_config_t;
enum BITRATE{CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS};
CAN_bit_timing_config_t can_configs[6] = {{2, 13, 45}, {2, 15, 20}, {2, 13, 18}, {2, 13, 9}, {2, 15, 4}, {2, 15, 2}};

void canInit(void) {
//	unsigned int brp = stm32_GetPCLK1();
// brp  = (brp / 18) / 500000;                     // baudrate is set to 500k bit/s

 	CAN_EN;              // enable clock for CAN
  GPIO_EN(A);          // enable GPIO clock for port used by CAN
  RCC->APB2ENR |= 0x1UL;
  // Setup CAN GPIO Pins
  GPIO_SET_MODE(A, CAN_RX_PIN, GPIO_MODE_AF);
  GPIO_SET_OTYPE(A, CAN_RX_PIN, GPIO_OTYPE_PUSHPULL);
  GPIO_SET_AF(GPIO_AFR_HI, A, 3, GPIO_AF_9);

  GPIO_SET_MODE(A, CAN_TX_PIN, GPIO_MODE_AF);
  GPIO_SET_OTYPE(A, CAN_TX_PIN, GPIO_OTYPE_PUSHPULL);
  GPIO_SET_AF(GPIO_AFR_HI, A, 4, GPIO_AF_9);  

  CAN_SET_MODE_INIT; // Put CAN into init mode
  CAN_ABOM_EN;       // Set Automatic Busoff management
  CAN_AWUM_EN;       // Set Automatic wakeup mode
  
  CAN->IER = (CAN_IER_FMPIE0 | CAN_IER_TMEIE);    // FIFO 0 msg pending, Transmit mbx empty

  /* set BTR register so that sample point is at about 72% bit time from bit start */
  /* TSEG1 = 12, TSEG2 = 5, SJW = 4 => 1 CAN bit = 18 TQ, sample at 72%    */
  // CAN->BTR |= CAN_BTR_SJW_1; // Set SJW to 1TQ
  // CAN->BTR |= CAN_BTR_TS1_2; // Set TS1 to 4TQ
  // CAN->BTR |= CAN_BTR_TS2_2; // Set TS2 to 4TQ
  // CAN->BTR |= CAN_BTR_BRP | 32-1U; // Set prescalar to 32

  // Setup CAN Filter
  CAN->FMR |= CAN_FMR_FINIT; // Put CAN filters in init mode
  CAN->FM1R &= ~CAN_FM1R_FBM0; // Set bank 0 to Mask Mode
  CAN->FS1R |= CAN_FS1R_FSC0; // Set to single 32 bit scale
  CAN->FFA1R &= ~CAN_FFA1R_FFA0; // Assign FIFO0
  CAN->sFilterRegister[0].FR1 = 0x0100 << 5; // Set Bank 0 Filter 1 (FIFO0) id filter
  CAN->FA1R |= CAN_FA1R_FACT0; // Activate FIFO0
	CAN->FMR &= ~CAN_FMR_FINIT;
  
  CAN->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
	CAN->BTR |=  (((can_configs[CAN_125KBPS].TS2-1) & 0x07) << 20) | (((can_configs[CAN_125KBPS].TS1-1) & 0x0F) << 16) | ((can_configs[CAN_125KBPS].BRP-1) & 0x1FF);

  // CAN->BTR &= ~(((        0x03) << 24) | ((        0x07) << 20) | ((         0x0F) << 16) | (          0x1FF)); 
  // CAN->BTR |=  ((((4-1) & 0x03) << 24) | (((5-1) & 0x07) << 20) | (((12-1) & 0x0F) << 16) | ((32-1) & 0x1FF));

  NVIC_EnableIRQ(CAN_TX_IRQn);  // Enable CAN TX Interrupt
  NVIC_EnableIRQ(CAN_RX0_IRQn); // Enable CAN RX Interrupt

  canStart();
}

/*----------------------------------------------------------------------------
  set the testmode
 *----------------------------------------------------------------------------*/
void canTestmode (unsigned int testmode) {

  CAN->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM);     // set testmode
  CAN->BTR |=  (testmode & (CAN_BTR_SILM | CAN_BTR_LBKM));
}

/*----------------------------------------------------------------------------
  check if transmit mailbox is empty
 *----------------------------------------------------------------------------*/
void canWaitReady (void)  {

  while ((CAN->TSR & CAN_TSR_TME0) == 0);         // Transmit mailbox 0 is empty
  CAN_TxRdy = 1;
 
}

/*----------------------------------------------------------------------------
  wite a message to CAN peripheral and transmit it
 *----------------------------------------------------------------------------*/
void canWrMsg (CAN_msg *msg)  {

  CAN->sTxMailBox[0].TIR  = (unsigned int)0;      // Reset TIR register
                                                  // Setup identifier information
  if (msg->format == STANDARD_FORMAT)  {          //    Standard ID
      CAN->sTxMailBox[0].TIR |= (unsigned int)(msg->id << 21) | msg->id;
  }  else  {                                      // Extended ID
      //CAN->sTxMailBox[0].TIR |= (unsigned int)(msg->id <<  3) | CAN_ID_EXT;
  }
                                                  // Setup type information
  if (msg->type == DATA_FRAME)  {                 // DATA FRAME
      CAN->sTxMailBox[0].TIR |= CAN_RTR_DATA;
  }
  else {                                          // REMOTE FRAME
      CAN->sTxMailBox[0].TIR |= CAN_RTR_REMOTE;
  }
                                                  // Setup data bytes
  CAN->sTxMailBox[0].TDLR = (((unsigned int)msg->data[3] << 24) | 
                             ((unsigned int)msg->data[2] << 16) |
                             ((unsigned int)msg->data[1] <<  8) | 
                             ((unsigned int)msg->data[0])        );
  CAN->sTxMailBox[0].TDHR = (((unsigned int)msg->data[7] << 24) | 
                             ((unsigned int)msg->data[6] << 16) |
                             ((unsigned int)msg->data[5] <<  8) |
                             ((unsigned int)msg->data[4])        );
                                                  // Setup length
  CAN->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
  CAN->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT0R_DLC);

  CAN->IER |= CAN_IER_TMEIE;                      // enable  TME interrupt 
  CAN->sTxMailBox[0].TIR |=  CAN_TI0R_TXRQ;       // transmit message
}

/*----------------------------------------------------------------------------
  read a message from CAN peripheral and release it
 *----------------------------------------------------------------------------*/
void canRdMsg (CAN_msg *msg)  {
                                                  // Read identifier information
  if ((CAN->sFIFOMailBox[0].RIR & CAN_ID_EXT) == 0) { // Standard ID
    msg->format = STANDARD_FORMAT;
    msg->id     = (uint32_t)0x000007FF & (CAN->sFIFOMailBox[0].RIR >> 21);
  }  else  {                                          // Extended ID
    // msg->format = EXTENDED_FORMAT;
    // msg->id     = (uint32_t)0x0003FFFF & (CAN->sFIFOMailBox[0].RIR >> 3);
  }
                                                  // Read type information
  if ((CAN->sFIFOMailBox[0].RIR & CAN_RTR_REMOTE) == 0) {
    msg->type =   DATA_FRAME;                     // DATA   FRAME
  }  else  {
    msg->type = REMOTE_FRAME;                     // REMOTE FRAME
  }
                                                  // Read length (number of received bytes)
  msg->len = (unsigned char)0x0000000F & CAN->sFIFOMailBox[0].RDTR;
                                                  // Read data bytes
  msg->data[0] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR);
  msg->data[1] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 8);
  msg->data[2] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 16);
  msg->data[3] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 24);

  msg->data[4] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR);
  msg->data[5] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 8);
  msg->data[6] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 16);
  msg->data[7] = (unsigned int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 24);

  CAN->RF0R |= CAN_RF0R_RFOM0;                    // Release FIFO 0 output mailbox
}


void canWrFilter (unsigned int id, unsigned char format, uint8_t canId)  {
  static unsigned short CAN_filterIdx = 0;
         unsigned int   CAN_msgId     = 0;
  
  if (CAN_filterIdx > 13) {                       // check if Filter Memory is full
    return;
  }
                                                  // Setup identifier information
  if (format == STANDARD_FORMAT)  {               // Standard ID
      CAN_msgId  |= (unsigned int)(id << 21) | canId;
  }  else  {                                      // Extended ID
      //CAN_msgId  |= (unsigned int)(id <<  3) | CAN_ID_EXT;
  }

  CAN->FMR  |=  CAN_FMR_FINIT;                    // set Initialisation mode for filter banks
  CAN->FA1R &=  ~(unsigned int)(1 << CAN_filterIdx); // deactivate filter

                                                  // initialize filter   
  CAN->FS1R |= (unsigned int)(1 << CAN_filterIdx);// set 32-bit scale configuration
  CAN->FM1R |= (unsigned int)(1 << CAN_filterIdx);// set 2 32-bit identifier list mode

  CAN->sFilterRegister[CAN_filterIdx].FR1 = CAN_msgId; //  32-bit identifier
  CAN->sFilterRegister[CAN_filterIdx].FR2 = CAN_msgId; //  32-bit identifier
    													   
  CAN->FFA1R &= ~(unsigned int)(1 << CAN_filterIdx);  // assign filter to FIFO 0
  CAN->FA1R  |=  (unsigned int)(1 << CAN_filterIdx);  // activate filter

  CAN->FMR &= ~CAN_FMR_FINIT;                     // reset Initialisation mode for filter banks

  CAN_filterIdx += 1;                             // increase filter index
}

/*----------------------------------------------------------------------------
  CAN transmit interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_TX_IRQHandler (void) {

  if (CAN->TSR & CAN_TSR_RQCP0) {                 // request completed mbx 0
    CAN->TSR |= CAN_TSR_RQCP0;                    // reset request complete mbx 0
    CAN->IER &= ~CAN_IER_TMEIE;                   // disable  TME interrupt
	
	CAN_TxRdy = 1; 
  }
}

/*----------------------------------------------------------------------------
  CAN receive interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_RX0_IRQHandler (void) {

  if (CAN->RF0R & CAN_RF0R_FMP0) {			      // message pending ?
	  canRdMsg (&CAN_RxMsg);                       // read the message

    CAN_RxRdy = 1;                                // set receive flag
  }
}


