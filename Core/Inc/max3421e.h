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

//USB stuff
uint8_t USB_enumerate(SPI_HandleTypeDef *hspi);
uint8_t USBdispatchToken(SPI_HandleTypeDef *hspi, uint8_t token, uint8_t ep, uint8_t naks);
uint8_t USBsetAddress(SPI_HandleTypeDef *hspi);
//TODO: write function to save toggle states and load them.
uint8_t USBsaveToggle(SPI_HandleTypeDef *hspi); //Not defined yet
uint8_t USBloadToggle(SPI_HandleTypeDef *hspi);
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
	uint8_t wLengthH
);

//D0NE: Configure the USB device,Get the whole descriptor
uint8_t USBgetDescriptor(SPI_HandleTypeDef *hspi);

#endif /* INC_MAX3421E_H_ */
