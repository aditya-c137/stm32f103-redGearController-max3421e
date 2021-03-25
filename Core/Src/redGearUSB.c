/*
 * redGearUSB.c
 *
 *  Created on: Mar 26, 2021
 *      Author: adity
 */

#include "max3421e.h"
#include "maxregisters.h"
#include "redGearUSB.h"
#include <stdio.h>
#include <stdlib.h>

/*

	RedGear XBOX controller specific hardcoded USB

*/

//
// TODO: SET_CONFIGURATION
//
// Follow these steps after setting device adddress:
//		1. Control transfer: set configuration 0x01 at addr 0x01 and ep0
//			1a. Load SETUP packet with data
//			1b. Dispatch packet beware of NAKS
//			1c. send HS-IN handshake... wait before sending this one ig



//
// TODO: Get the HID report Descriptor
//
//		1(Optional) i guess. set Idle (HID), All... TODO: what is this???
//		2. Control Transfer: get HID Report Descriptor at adde 0x01 and ep0 data size 0x81
//			2a. Load SETUP packet with data
//			2b. Dispatch packet beware of NAKS
//			2c. Send bulk in request.
//				2c1. wait for bmRCVDAVIRQ to set 
//					 FIXME: this stage is really buggy... a nightmare
//				2c2. reset bmRCVDAVIRQ immediately 
//					 (TODO: might have to check bmRCVDAVIRQ immediately...
//					 for second buffer... not sure further reading needed)
//				2c3. load RCVBC bytes into memory
//			2d. send HS-OUT handshake... wait before sending this one i guess
