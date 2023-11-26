/*
 * max3421e.h
 *
 *  Created on: Mar 15, 2021
 *      Author: adity
 */

#ifndef INC_MAX3421E_H_
#define INC_MAX3421E_H_

#include "stm32f1xx_hal.h"

//related datatypes
int vbusState;
typedef struct {
	uint8_t epAddr;
	uint8_t maxPktSize;

	union {
		uint8_t epAttribs;

		struct {
			uint8_t bmSndToggle : 1;
			uint8_t bmRcvToggle : 1;
			uint8_t bmNakPower: 6;
		} __attribute__((packed));
	};
}__attribute__((packed)) EpInfo;


typedef struct {
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint8_t wValueLo;
	uint8_t wValueHi;
	uint8_t wIndexLo;
	uint8_t wIndexHi;
	uint8_t wLengthLo;
	uint8_t wLengthHi;
}__attribute__((packed)) SETUP_PKT, *PSETUP_PKT;


EpInfo allEp[3];


//function prototypes MAX3421e init and stuff
void regWr(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data);
void regWrN(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, int nbytes);
uint8_t regRd(SPI_HandleTypeDef *hspi, uint8_t reg);
void regRdN(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, int nbytes);
uint8_t revisionRd(SPI_HandleTypeDef *hspi);
uint8_t maxInit(SPI_HandleTypeDef *hspi);
uint8_t maxBusprobe(SPI_HandleTypeDef *hspi);
uint8_t maxIntHandler(SPI_HandleTypeDef *hspi);
uint8_t maxTask(SPI_HandleTypeDef *hspi);
uint16_t maxReset(SPI_HandleTypeDef *hspi);
uint8_t maxTask(SPI_HandleTypeDef *hspi);
uint8_t maxIntHandler(SPI_HandleTypeDef *hspi);
uint8_t maxSetAddress(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, EpInfo **ppep, uint16_t *naks);

//USB stuff
uint8_t USB_enumerate(SPI_HandleTypeDef *hspi);
uint8_t USBdispatchPacket(SPI_HandleTypeDef *hspi, uint8_t token, uint8_t ep, uint16_t naks);
uint8_t USBsetAddress(SPI_HandleTypeDef *hspi);
//TODO: USBsetupDispatch write function for this
uint8_t USBsetupDispatch(
	SPI_HandleTypeDef *hspi,
	uint8_t bmRequestType,
	uint8_t bRequest,
	uint8_t wValueL,
	uint8_t wValueH,
	uint8_t wIndexL,
	uint8_t wIndexH,
	uint8_t wLengthL,
	uint8_t wLengthH,
	int naks
);
uint8_t USBctrlReq(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, uint8_t bmReqType, uint8_t bRequest, uint8_t wValLo, uint8_t wValHi, uint8_t wIndLo, uint8_t wIndHi, uint8_t wLenLo, uint8_t wLenHi, uint16_t nbytes, uint8_t* dataptr);

uint8_t USBinTransfer(SPI_HandleTypeDef *hspi, EpInfo *pep, uint16_t naks, uint16_t *nbytesptr, uint8_t* data, uint8_t bInterval /*= 0*/);
uint8_t USBoutTransfer(SPI_HandleTypeDef *hspi, EpInfo *pep, uint16_t naks, uint16_t nbytes, uint8_t* data);
EpInfo* USBgetEpInfoEntry(uint8_t ep);
uint8_t USBin_Transfer(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t ep, uint16_t *nbytesptr,uint8_t* data, uint8_t bInterval);



void setAllEp();

//D0NE: Configure the USB device,Get the whole descriptor
uint8_t USBgetDescriptor(SPI_HandleTypeDef *hspi);

#endif /* INC_MAX3421E_H_ */
