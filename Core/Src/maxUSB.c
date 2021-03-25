/*
 * maxUSB.c
 *
 *  Created on: Mar 25, 2021
 *      Author: adity
 */


#include "max3421e.h"
#include "maxregisters.h"
#include <stdio.h>
#include <stdlib.h>


//RedGear XBOXUSB
#define MAX_PACKET_SIZE 40

uint8_t USB_enumerate(SPI_HandleTypeDef *hspi){
	//	/*
	//	 *
	//	 * BUS RESET
	//	 *
	//	 *
	//	 * */
	uint8_t temp = regRd(hspi,rHCTL);
	temp |= 0x01;
	regWr(hspi,rHCTL,temp);
	while(regRd(hspi,rHCTL)&bmBUSRST);
//	regWr(hspi,rHIRQ,(regRd(hspi,rHIRQ)&0b11111110));
	regWr(hspi,rMODE,(bmDMPULLDN|bmDPPULLDN|bmHOST|bmSOFKAENAB));
	while(!(regRd(hspi,rHIRQ)&bmFRAMEIRQ));

	//send packet to set the USB address
	USBsetAddress(hspi);
	HAL_Delay(20);

	//write newly assigned USB address for future addressing
	regWr(hspi,rPERADDR,0x01);

	return 0;
}

uint8_t USBdispatchToken(SPI_HandleTypeDef *hspi, uint8_t token, uint8_t ep, uint8_t naks){
	while(naks){
		regWr(hspi,rHXFR,(token | ep));
		// printf("waiting for HXFRDNIRQ...\n");
		while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));
//		printf("dispatch HRSL: %x\n",regRd(hspi,rHRSL));
		if((regRd(hspi,rHRSL)&0x0f) == 0x04){
			naks--;
		}
		else{
			break;
		}
	}
	return regRd(hspi,rHRSL)&0x0f;
}

uint8_t USBsetAddress(SPI_HandleTypeDef *hspi){
	uint8_t naks = 10;
	//setup packet
	uint8_t setupPacket[8] = {
			0x00, //bit 7 = 1 host to device
			0x05, //SET_ADDRESS
			0x01, //~
			0x00, //Device address
			0x00, //index not relevant
			0x00,
			0x00, //lenght to be retrived
			0x00
	};
	while(naks){
		regWrN(hspi,rSUDFIFO,setupPacket,8);

		USBdispatchToken(hspi,0x10,0x00,50);

		//handshake
		//HS-IN
		regWr(hspi,rHXFR,0x80);
		while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));
		if((regRd(hspi,rHRSL)&0x0f) == 0x04){
			naks--;
		}
		else {
			break;
		}
	}
	return regRd(hspi,rHRSL);
}

uint8_t USBgetDescriptor(SPI_HandleTypeDef *hspi){
	uint8_t temp;
	/*
		SETUP stage
	*/
	//send a setup packet
	//D0NE: find what 8 bytes constitute a setup packet data
	//1. write 8 bytes of setup data to SUDFIFO
	uint8_t setupPacket[8] = {
			0x80, //bit 7 = 1 device to host
			0x06, //GET_DESCRIPTOR
			0x00, //~
			0x02, //descriptor type
			0x00, //index not relevant
			0x00,
			0x30, //lenght to be retrived
			0x00
	};

	regWrN(hspi,rSUDFIFO,setupPacket,8);
	//2. Load the HXFR register with 00010000 this will initiate the transfer
	regWr(hspi,rHXFR,0x10);
	HAL_Delay(1);
	//3. wait for HXFRDNIRQ bit to be set appx 18 bit times
	// printf("waiting for HXFRDNIRQ...\n");
	while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));
	//4. check the HRSLT[3:0] bits for result of transfer
	regRd(hspi,rHRSL);

	/*
		DATA stage
	*/
	//send bulk-in request
	//send token
	USBdispatchToken(hspi,0x00,0x00,50);
	uint8_t RCVBC;
//	while(!(regRd(hspi,rHIRQ)&bmRCVDAVIRQ)){
//		USBdispatchToken(hspi,0x00,0x00,50);
//		// This is a very dangerous hack!
//		// Correct solution needs to be found
//		/*
//		 *
//		 *
//		 *
//		 *NEVER DO THIS
//		 *
//		 *
//		 *
//		 *
//		 */
//		NVIC_SystemReset(); //FIXME: Do not reset like this... FIND A DIFFERENT SOLUTION
//		/*
//		 *
//		 *
//		 *
//		 *
//		 *PLEASE DON'T RESET LIKE THIS
//		 *
//		 *
//		 *
//		 *
//		 */
//	}
	if(regRd(hspi,rHIRQ)&bmRCVDAVIRQ){
		temp = regRd(hspi,rHIRQ);
		temp &= 0xfb; //We need to set the bmRCVDAVIRQ bit back to 0 after reading
		regWr(hspi,rHIRQ,temp);
		RCVBC = regRd(hspi,rRCVBC);
		printf("RCVBC: %x\n",RCVBC);

	}
	else {
		return 0xff;
	}
	//Read RCVFIFO
	uint8_t DESC[40];
	regRdN(hspi,rRCVFIFO,DESC,RCVBC);
	for(int i = 0; i < RCVBC; i++){
		printf("%x\n",DESC[i]);
	}
//	printf("bLength: %x\n",DESC[0]);
//	printf("bDescriptorType: %x\n",DESC[1]);
//	printf("bcdUSB: %x%x\n",DESC[3],DESC[2]);
//	printf("bDeviceClass: %x\n",DESC[4]);
//	printf("bDeviceSubClass: %x\n",DESC[5]);
//	printf("bDeviceProtocol: %x\n",DESC[6]);
//	printf("bMaxPacketSize0: %x\n",DESC[7]);
//	printf("idVendor: %x%x\n",DESC[9],DESC[8]);
//	printf("idProduct: %x%x\n",DESC[11],DESC[10]);
//	printf("bcdDevice: %x%x\n",DESC[13],DESC[12]);
//	printf("iManufacturer: %x\n",DESC[14]);
//	printf("iProduct: %x\n",DESC[15]);
//	printf("iSerialNumber: %x\n",DESC[16]);
//	printf("bNumConfigurations: %x\n",DESC[17]);
	/*
		STATUS stage
	*/
	//send HS-OUT
	regWr(hspi,rHXFR,0xA0);
	HAL_Delay(1);
	// printf("waiting for HXFRDNIRQ...\n");
	while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));

	return regRd(hspi,rHRSL)&0x0F;
}
