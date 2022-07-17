#ifndef CAN_H
#define CAN_H

#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1

#define DATA_FRAME       0
#define REMOTE_FRAME     1

typedef unsigned char uint8_t;

typedef struct  {
  unsigned int   id;                 // 29 bit identifier
  unsigned char  data[8];            // Data field
  unsigned char  len;                // Length of data field in bytes
  unsigned char  format;             // 0 - STANDARD, 1- EXTENDED IDENTIFIER
  unsigned char  type;               // 0 - DATA FRAME, 1 - REMOTE FRAME
} CAN_msg;

extern volatile CAN_msg       CAN_TxMsg;      // CAN messge for sending

/* Functions defined in module CAN.c */
void canSetup         (void);
void canInit          (void);
void canStart         (void);
void canWaitReady     (void);
void canWrMsg         (CAN_msg *msg);
void canRdMsg         (CAN_msg *msg);
void canWrFilter      (unsigned int id, unsigned char filter_type, uint8_t canId);

void CAN_testmode      (unsigned int testmode);



#endif // CAN_H


