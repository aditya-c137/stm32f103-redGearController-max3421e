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
#include <stdbool.h>


//RedGear XBOXUSB
#define MAX_PACKET_SIZE 40

uint8_t setupPacket[8] = {0,0,0,0,0,0,0,0};

void setAllEp(){
	allEp[0].epAddr = 0x00;
	allEp[0].maxPktSize = 0x40;
	allEp[0].bmNakPower = 0x0e;
	allEp[1].epAddr = 0x01;
	allEp[1].epAttribs = 0x03;
	allEp[1].bmSndToggle = 0x00;
	allEp[1].bmRcvToggle = 0x00;
	allEp[1].maxPktSize = 0x20;
	allEp[1].bmNakPower = 0x01;
	allEp[2].epAddr = 0x02;
	allEp[2].epAttribs = 0x03;
	allEp[2].bmSndToggle = 0x00;
	allEp[2].bmRcvToggle = 0x00;
	allEp[2].maxPktSize = 0x20;
	allEp[2].bmNakPower = 0x01;
}

uint8_t USB_enumerate(SPI_HandleTypeDef *hspi){
	//	/*
	//	 *
	//	 * BUS RESET
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

	allEp[0].epAddr = 0x00;
	allEp[0].maxPktSize = 0x40;
	allEp[0].bmNakPower = 0x0e;
	allEp[1].epAddr = 0x01;
	allEp[1].epAttribs = 0x03;
	allEp[1].bmSndToggle = 0x00;
	allEp[1].bmRcvToggle = 0x00;
	allEp[1].maxPktSize = 0x20;
	allEp[1].bmNakPower = 0x01;
	allEp[2].epAddr = 0x02;
	allEp[2].epAttribs = 0x03;
	allEp[2].bmSndToggle = 0x00;
	allEp[2].bmRcvToggle = 0x00;
	allEp[2].maxPktSize = 0x20;
	allEp[2].bmNakPower = 0x01;

	return 0;
}

uint8_t USBdispatchPacket(SPI_HandleTypeDef *hspi, uint8_t token, uint8_t ep, uint16_t naks){
	uint32_t timeout = HAL_GetTick() + 5000;
	uint8_t tmpdata;
	uint8_t rcode = 0x00;
	uint8_t retry_count = 0;
	uint16_t nak_count = 0;

	while((int32_t)((HAL_GetTick() - timeout) < 0L)){
		//lauch transfer
		regWr(hspi,rHXFR, (token|ep));
		rcode = 0xff;

		//wait for transfer completion
		while((int32_t)(HAL_GetTick() - timeout) < 0L){
			tmpdata = regRd(hspi,rHIRQ);

			if(tmpdata & bmHXFRNDIRQ) {
				regWr(hspi,rHIRQ,bmHXFRNDIRQ); //clear interrupt
				rcode = 0x00;
				break;
			}
		}

		rcode = (regRd(hspi,rHRSL) & 0x0f); //transfer result

		switch(rcode){
			case 0x04:
				nak_count++;
				if(naks && (nak_count == naks))
					return(rcode);
				break;
			case 0x0e:
				retry_count++;
				if(retry_count == 3)
					return rcode;
				break;
			default:
				return rcode;
		}
	}

	return rcode;


// 	while(naks){
// 		regWr(hspi,rHXFR,(token | ep));
// 		// printf("waiting for HXFRDNIRQ...\n");
// 		while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));
// //		printf("dispatch HRSL: %x\n",regRd(hspi,rHRSL));
// 		if((regRd(hspi,rHRSL)&0x0f) == 0x00){
// 			break;
// 		}
// 		else if((regRd(hspi,rHRSL)&0x0f) == 0x04){
// 			naks--;
// 		}
// 	}
// 	return regRd(hspi,rHRSL)&0x0f;
}

uint8_t USBsetAddress(SPI_HandleTypeDef *hspi){
	// uint8_t naks = 10;
	// //setup packet
	// uint8_t setupPacket[8] = {
	// 		0x00, //bit 7 = 1 host to device
	// 		0x05, //SET_ADDRESS
	// 		0x01, //~
	// 		0x00, //Device address
	// 		0x00, //index not relevant
	// 		0x00,
	// 		0x00, //lenght to be retrived
	// 		0x00
	// };

	return USBctrlReq(hspi,0x00,0x00,0x00,0x05,0x01,0x00,0x00,0x00,0x00,0x00,0x00,NULL);

	// while(naks){
	// 	regWrN(hspi,rSUDFIFO,setupPacket,8);

	// 	USBdispatchPacket(hspi,0x10,0x00,50);

	// 	//handshake
	// 	//HS-IN
	// 	regWr(hspi,rHXFR,0x80);
	// 	while(!(regRd(hspi,rHIRQ) & bmHXFRNDIRQ));
	// 	if((regRd(hspi,rHRSL)&0x0f) == 0x04){
	// 		naks--;
	// 	}
	// 	else {
	// 		break;
	// 	}
	// }
	// return regRd(hspi,rHRSL);
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
//	USBdispatchPacket(hspi, 0x10, 0x00, 10);

	/*
		DATA stage
	*/
	//send bulk-in request
	//send token
	USBdispatchPacket(hspi,0x00,0x00,50);
	uint8_t RCVBC;
//	while(!(regRd(hspi,rHIRQ)&bmRCVDAVIRQ)){
//		USBdispatchPacket(hspi,0x00,0x00,50);
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

uint8_t USBsetupDispatch(SPI_HandleTypeDef *hspi,
	uint8_t bmRequestType,
	uint8_t bRequest,
	uint8_t wValueL,
	uint8_t wValueH,
	uint8_t wIndexL,
	uint8_t wIndexH,
	uint8_t wLengthL,
	uint8_t wLengthH,
	int naks
){
	setupPacket[0] = bmRequestType;
	setupPacket[1] = bRequest;
	setupPacket[2] = wValueL;
	setupPacket[3] = wValueH;
	setupPacket[4] = wIndexL;
	setupPacket[5] = wIndexH;
	setupPacket[6] = wLengthL;
	setupPacket[7] = wLengthH;
	regWrN(hspi,rSUDFIFO,setupPacket,8);
	// while(naks){
	// 	regWr(hspi,rHXFR,0x10);
	// 	while(!(regRd(hspi,rHIRQ)&bmHXFRNDIRQ));
	// 	if(regRd(hspi,rHRSL)&0x0f == 0x00){
	// 		break;
	// 	}
	// 	else if(regRd(hspi,rHRSL)&0x0f == 0x04){
	// 		nak--;
	// 	}
	// }
	// return regRd(hspi,rHRSL)&0x0f;
	
	if(USBdispatchPacket(hspi,0x10,0x00,naks) != 0){
		return 0x33;
	}

	return USBdispatchPacket(hspi,0x80,0x00,naks);
}


EpInfo* USBgetEpInfoEntry(uint8_t ep){
	EpInfo *pep = allEp;
	for(uint8_t i = 0; i < 3; i++){
		if((pep)->epAddr == ep)
			return pep;
		pep++;
	}
	return NULL;
}


uint8_t maxSetAddress(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, EpInfo **ppep, uint16_t *naks){
	*(ppep) = USBgetEpInfoEntry(ep);

	if(!*ppep){
		return 0xdb;
	}

	*naks = (0x0001UL << (((*ppep)->bmNakPower > 15) ? 15 : (*ppep)->bmNakPower));
	(*naks)--;

	regWr(hspi, rPERADDR, addr);

	return 0x00;
}

uint8_t USBctrlReq(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, uint8_t bmReqType, uint8_t bRequest, uint8_t wValLo, uint8_t wValHi, uint8_t wIndLo, uint8_t wIndHi, uint8_t wLenLo, uint8_t wLenHi, uint16_t nbytes, uint8_t* dataptr){
	uint8_t rcode;
	SETUP_PKT setup_pkt;
	uint16_t naks = 0;
	bool direction = false;
	uint16_t total = 0x0000; 
	total |= wLenHi;
	total = total << 8;
	total |= wLenLo;
	EpInfo *pep = NULL;

	rcode = maxSetAddress(hspi, addr, ep, &pep, &naks);

	regWr(hspi, rPERADDR, addr);

	direction = ((bmReqType & 0x80) > 0);

	setup_pkt.bmRequestType = bmReqType;
	setup_pkt.bRequest = bRequest;
	setup_pkt.wValueLo = wValLo;
	setup_pkt.wValueHi = wValHi;
	setup_pkt.wIndexLo = wIndLo;
	setup_pkt.wIndexHi = wIndHi;
	setup_pkt.wLengthLo = wLenLo;
	setup_pkt.wLengthHi = wLenHi;

	regWrN(hspi, rSUDFIFO, (uint8_t*) &setup_pkt, 8);

	rcode = USBdispatchPacket(hspi, 0x10, ep, naks);

	if(rcode)
		return rcode;
	//data stage
	if(dataptr != NULL){
		if(direction){
			uint16_t left = total;
			
			pep->bmRcvToggle = 1;

			while(left) {
				uint16_t read = nbytes;
				rcode = USBinTransfer(hspi,pep,naks,&read,dataptr,0);
				if(rcode == 0x06) {
					pep->bmRcvToggle = (regRd(hspi,rHRSL) & bmSNDTOGRD) ? 0 : 1;
					continue;
				}
				if(rcode)
					return rcode;
				left -= read;

				if(read<nbytes)
					break;
			}
		}
		else{
			pep->bmSndToggle = 1;
			rcode = USBoutTransfer(hspi,pep,naks,nbytes,dataptr);
		}
		if(rcode)
			return rcode;
	}
	return USBdispatchPacket(hspi,(direction)?0xa0:0x80,ep,naks);
}


uint8_t USBinTransfer(SPI_HandleTypeDef *hspi, EpInfo *pep, uint16_t naks, uint16_t *nbytesptr, uint8_t* data, uint8_t bInterval /*= 0*/){
	uint8_t rcode = 0;
	uint8_t pktsize;
	uint16_t nbytes = *nbytesptr;
	uint8_t maxpktsize = pep->maxPktSize;

	*nbytesptr = 0;
	regWr(hspi, rHCTL, (pep->bmRcvToggle) ? bmRCVTOG1 : bmRCVTOG0);

	while(1) {
		rcode = USBdispatchPacket(hspi, 0x00, pep->epAddr, naks);
		printf("epaddress: %x\n",pep->epAddr);
		if(rcode == 0x06) {
			pep->bmRcvToggle = regRd(hspi, rHRSL & bmRCVTOGRD) ? 0 : 1;
			regWr(hspi, rHCTL, (pep->bmRcvToggle) ? bmRCVTOG1 : bmRCVTOG1);
			continue;
		}
		if(rcode) {
			break;
		}
		if((regRd(hspi, rHIRQ) & bmRCVDAVIRQ) == 0) {
			printf("HIRQ: %x\n",regRd(hspi, rHIRQ));
			rcode = 0xf0;
			break;
		}
		pktsize = regRd(hspi, rRCVBC);
		if(pktsize > nbytes) {
			pktsize = nbytes;
		}

		int16_t mem_left = (int16_t)nbytes - *((int16_t*)nbytesptr);

		if(mem_left < 0)
			mem_left = 0;
		
		regRdN(hspi,rRCVFIFO,data,((pktsize > mem_left) ? mem_left : pktsize));

		regWr(hspi, rHIRQ, bmRCVDAVIRQ);
		*nbytesptr += pktsize;

		if((pktsize < maxpktsize) || (*nbytesptr >= nbytes)) {
			pep->bmRcvToggle = ((regRd(hspi, rHRSL) & bmRCVTOGRD)) ? 1 : 0;
			rcode = 0;
			break;
		}
		else if(bInterval > 0)
			HAL_Delay(bInterval);
	}
	return rcode;
}


uint8_t USBoutTransfer(SPI_HandleTypeDef *hspi, EpInfo *pep, uint16_t naks, uint16_t nbytes, uint8_t* data){
	uint8_t rcode  = 0x00, retry_count;
	uint8_t *data_p = data;
	uint16_t bytes_tosend, nak_count;
	uint16_t bytes_left = nbytes;

	uint8_t maxpktsize = pep->maxPktSize;

	if(maxpktsize < 1 || maxpktsize > 64)
		return 0xda;
	
	uint32_t timeout = HAL_GetTick() + 5000;

	regWr(hspi,rHCTL, (pep->bmSndToggle) ? bmSNDTOG1 : bmSNDTOG0);

	while(bytes_left) {
		retry_count = 0;
		nak_count = 0;
		bytes_tosend = (bytes_left >= maxpktsize) ? maxpktsize : bytes_left;
		regWrN(hspi, rSNDFIFO, data_p, bytes_tosend);
		regWr(hspi, rSNDBC, bytes_tosend);
		regWr(hspi, rHXFR, (0x20 | pep->epAddr));
		while(!(regRd(hspi, rHIRQ) & bmHXFRNDIRQ));
		regWr(hspi, rHIRQ, bmHXFRNDIRQ);
		rcode = (regRd(hspi,rHRSL) & 0x0f);
		while(rcode && ((int32_t)(HAL_GetTick() - timeout) < 0L)) {
			switch(rcode) {
				case 0x04:
					nak_count++;
					if(naks && (nak_count == naks))
						goto breakout;
					break;
				case 0x0e:
					retry_count++;
					if(retry_count == 3)
						goto breakout;
					break;
				case 0x06:
					pep->bmSndToggle = (regRd(hspi, rHRSL) & bmSNDTOGRD) ? 0 : 1;
					regWr(hspi, rHCTL, (pep->bmSndToggle) ? bmSNDTOG1 : bmSNDTOG0);
					break;
				default:
					goto breakout;
			}

			regWr(hspi, rSNDBC, 0);
			regWr(hspi, rSNDFIFO, *data_p);
			regWr(hspi, rSNDBC, bytes_tosend);
			regWr(hspi, rHXFR, (0x20 | pep->epAddr));
			while(!(regRd(hspi, rHIRQ) & bmHXFRNDIRQ));
			regWr(hspi, rHIRQ,bmHXFRNDIRQ);
			rcode = (regRd(hspi, rHRSL) & 0x0f);
		}
		bytes_left -= bytes_tosend;
		data_p += bytes_tosend;
	}
breakout:
	pep->bmSndToggle = (regRd(hspi, rHRSL) & bmSNDTOGRD) ? 1 : 0;
	return rcode;
}

uint8_t USBin_Transfer(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, uint16_t *nbytesptr,uint8_t* data, uint8_t bInterval) {
	EpInfo *pep = NULL;
	uint16_t naks = 0;

	uint8_t rcode = maxSetAddress(hspi,addr,ep,&pep,&naks);

	if(rcode){
		return rcode;
	}
	return USBinTransfer(hspi,pep,naks,nbytesptr,data,bInterval);
}