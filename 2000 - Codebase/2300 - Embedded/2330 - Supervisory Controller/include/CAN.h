/*
 * 
 *	Name: 			CAN.h
 *	Author:			Garnet Koebel, adapted from examples by Micheal Galle
 *	Version: 		V0.3
 *	Created:		Refer to source control
 *	Last Update:	Refer to source control
 *	Description:	This file declares the globally exposed portions of the
 *                  elevator CAN protocol.
 *
 */

#ifndef CAN_H
#define CAN_H
#include <libpcan.h>

// Function declarations
TPCANMsg pcanRx(void);
int pcanTx(int id, int data);


#endif
