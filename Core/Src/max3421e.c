/*
 * max3421e.c
 *
 *  Created on: Mar 15, 2021
 *      Author: adity
 */

#include "max3421e.h"
#include "maxregisters.h"
#include <stdio.h>
#include <stdlib.h>

//RedGear XBOXUSB
#define MAX_PACKET_SIZE 40

//Write a register
void regWr(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	uint8_t txData = reg | 0x02;
	HAL_SPI_Transmit(hspi, &txData, 1, HAL_MAX_DELAY);
	txData = data;
	HAL_SPI_Transmit(hspi, &txData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

//Write nbytes to a register
void regWrN(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, int nbytes)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	uint8_t txData = reg | 0x02;
	HAL_SPI_Transmit(hspi, &txData, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, data, nbytes, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

//read a byte from a register
uint8_t regRd(SPI_HandleTypeDef *hspi, uint8_t reg)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	//register to read
	uint8_t rxData = reg;
	HAL_SPI_Transmit(hspi, &rxData, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, &rxData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	return rxData;
}

void regRdN(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t* data, int nbytes){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	//register to read
	uint8_t rxData = reg;
	HAL_SPI_Transmit(hspi, &rxData, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, data, nbytes, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

//read the revision register
uint8_t revisionRd(SPI_HandleTypeDef *hspi)
{
	//set the host shield for full duplex mode and return the read only
	//revision registor to check if the operation was successful
	//if return value is 0 - unsuccessful
	//if 19 (version of board on 15 march 2021) - successful
	regWr(hspi, rPINCTL, bmFDUPSPI | bmINTLEVEL | bmGPXA);
	return regRd(hspi, rREVISION); //Read-Only revision register
}

uint16_t maxReset(SPI_HandleTypeDef *hspi)
{
	uint16_t i = 0;
	regWr(hspi, 0x78, 0x20);
	regWr(hspi, 0x78, 0x00);
	while(++i)
	{
		if(regRd(hspi,0x68) & 0x01)
		{
			break;
		}
	}
	return i;
}

//returns -1 on failure 0 on success
uint8_t maxInit(SPI_HandleTypeDef *hspi)
{
	//set host shield in full duplex spi mode and verify
	regWr(hspi, rPINCTL, bmFDUPSPI | bmINTLEVEL | bmGPXA);
	if (0x13 == regRd(hspi, rREVISION))
	{
		//set host shield in host mode and pull ups
		// regWr(hspi, 0xd8, 0xc1);

//		return maxReset(hspi);

        if(maxReset(hspi) == 0) { //OSCOKIRQ hasn't asserted in time
                return 0xfc;
        }

		//set host shield in host mode and pull ups
		regWr(hspi, 0xd8, 0xc1);
		
		//detect connection
		regWr(hspi, 0xd0, 0x60);

		regWr(hspi, 0xe8, 0x04);
		int i = regRd(hspi, 0xe8) & 0x04;
		while (!i){
			i = regRd(hspi, 0xe8) & 0x04;
			printf("i = %x\n",i);
		}

		//busprobe();
		maxBusprobe(hspi);
		//busprobe will read j and k status set speed depending on this

		//clear connection detect interrupt
		regWr(hspi,rHCTL,bmCONDENTIRQ);
		//enable interrupt pin
		regWr(hspi,rCPUCTL, 0x01);

		//busprobe fails if switch is connected, feels irrelevant, what is a switch
		//regWr(hspi,rPINCTL, (bmFDUPSPI | bmINTLEVEL));

		return (0);
	}
	else
	{
		return 0xde;
	}
}

uint8_t maxBusprobe(SPI_HandleTypeDef *hspi)
{
	uint8_t HRSL = regRd(hspi, rHRSL);
	HRSL &= 0xc0; //make everything except J-K 0
	HRSL = HRSL >> 6; //shift bits to LSB
	switch(HRSL) {
		case 0x00:
            regWr(hspi,rMODE, bmDPPULLDN | bmDMPULLDN | bmHOST | bmSEPIRQ);
			return 0xfd;
			// break;
		case 0x01:
			// k state
			if(regRd(hspi,rMODE) & bmLOWSPEED){
				//full speed
				regWr(hspi, rMODE, 0x80|0x40|0x08|0x01);
				vbusState = 2;
				printf("D+ high and D- low: %x\n",regRd(hspi,rMODE));
			}
			else {
				//low speed
				printf("D+ low and D- high: %x\n",regRd(hspi,rMODE));
			}
			return 0x02;
			// break;
		case 0x02:
			//j state
			if(regRd(hspi,rMODE) & bmLOWSPEED){
				//low speed
				printf("D+ low and D- high: %x\n",regRd(hspi,rMODE));
			}
			else {
				//full speed
				vbusState = 2;
				regWr(hspi, rMODE, 0x80|0x40|0x08|0x01);
				printf("D+ high and D- low: %x\n",regRd(hspi,rMODE));
			}
			return 0x03;
			// break;
		case 0x03:
			return 0x04;
			// break;
		default:
			return 0xbc;
	}
}

uint8_t maxTask(SPI_HandleTypeDef *hspi){
	uint8_t rcode = 0;
	uint8_t pinvalue;

	//read INT pin
	pinvalue = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);

	if(pinvalue == 0){
		rcode = maxIntHandler(hspi);
	}

	return rcode;
}

uint8_t maxIntHandler(SPI_HandleTypeDef *hspi){
	uint8_t HIRQ;
	uint8_t HIRQ_sendback = 0x00;
	HIRQ = regRd(hspi,rHIRQ);
	if(HIRQ & bmCONDENTIRQ){
		maxBusprobe(hspi);
		HIRQ_sendback |= bmCONDENTIRQ;
	}
	regWr(hspi,rHIRQ,HIRQ_sendback);
	return HIRQ_sendback;
}

