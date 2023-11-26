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


uint8_t readBuf[32];



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
uint8_t RGsetConfiguration(SPI_HandleTypeDef *hspi)
{
	//1a. and 1b.
	// return USBsetupDispatch(hspi,
	// 				 0x00, //host to device
	// 				 0x09, //set configuration
	// 				 0x01, //configuration value Low byte
	// 				 0x00, //configuration value High byte
	// 				 0x00, //wIndexL
	// 				 0x00, //wIndexH
	// 				 0x00, //wLengthL
	// 				 0x00,  //wLengthH
	// 				 50
	// );
	return USBctrlReq(hspi,0x01,0x00,0x00,0x09,0x01,0x00,0x00,0x00,0x00,0x00,0x00,NULL);

	//1c.

}

//
// TODO: Get the HID report Descriptor
//
//		1(Optional) i guess. set Idle (HID), All... TODO: what is this???
//		2. Control Transfer: get HID Report Descriptor Type 0x24 at adde 0x01 and ep0 data size 0x81 pretty sure this is going to cause STALL
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
uint8_t RGgetHIDreportDescriptor(SPI_HandleTypeDef *hspi){
	//2a. and 2b.
	USBsetupDispatch(hspi,
					 0x81, //host to device
					 0x06, //set configuration
					 0x21, //wValue Low byte
					 0x00, //wValue High byte
					 0x00, //wIndexL
					 0x00, //wIndexH
					 0x14, //wLengthL
					 0x00, //wLengthH
					 50
	);

	//2c.
	uint8_t RCVBC;
	do{
		USBdispatchPacket(hspi,0x01,0x00,50);
		uint8_t temp = regRd(hspi,rHIRQ);
		temp &= 0xfb;
		regWr(hspi,rHIRQ,temp);
		RCVBC = regRd(hspi,rRCVBC);
		printf("HID RCVBC: %x\n",RCVBC);
	}while(regRd(hspi,rHIRQ)&bmRCVDAVIRQ);

	uint8_t DESC[81];
	regRdN(hspi,rRCVFIFO,DESC,RCVBC);
	for(int i = 0; i < RCVBC; i++){
		printf("%x\n",DESC[i]);
	}

	regWr(hspi,rHXFR,0xa0);
	HAL_Delay(200);
	while(!(regRd(hspi,rHIRQ)&bmHXFRNDIRQ));

	return regRd(hspi,rHRSL)&0x0f;
}

uint8_t RGpoll(SPI_HandleTypeDef *hspi){
	uint16_t BUFFER_SIZE = 0x0020;
	return USBin_Transfer(hspi,0x01,0x01,&BUFFER_SIZE,readBuf,0x04);
}

uint8_t RGinit(SPI_HandleTypeDef *hspi){
	uint8_t buf[0x14];
	uint8_t rcode;

	setAllEp();
	//get descriptor
	rcode = USBctrlReq(hspi,0x00,0x00,0x80,0x06,0x00,0x01,0x00,0x00,0x14,0x00,0x0014,buf);
	if (rcode)
		return rcode;
	allEp[0].maxPktSize = 0x40;
	EpInfo *pep = NULL;
	uint16_t naks;
	//set address
	rcode = USBctrlReq(hspi,0x00,0x00,0x00,0x05,0x01,0x00,0x00,0x00,0x00,0x00,0,NULL);
	maxSetAddress(hspi,0x01,0x00,&pep,&naks);
	if (rcode)
		return rcode;

	return rcode;
}

void RGcommand(SPI_HandleTypeDef *hspi, uint8_t* data, uint16_t nbytes) {
	//bmRequest = Host to device (0x00) | Class (0x20) | Interface (0x01) = 0x21, bRequest = Set Report (0x09), Report ID (0x00), Report Type (Output 0x02), interface (0x00), datalength, datalength, data)
	uint8_t nbyteslo = (uint8_t)(nbytes & 0x00ff);
	uint8_t nbyteshi = (uint8_t)((nbytes>>8)&0x00ff);
	USBctrlReq(hspi, 0x01, 0x00, 0x21, 0x09, 0x00, 0x02, 0x00, 0x00, nbyteslo, nbyteshi,nbytes, data);
}