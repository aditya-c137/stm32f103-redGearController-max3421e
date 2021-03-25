/*
 * redGearUSB.h
 *
 *  Created on: Mar 26, 2021
 *      Author: adity
 */

#ifndef INC_REDGEARUSB_H_
#define INC_REDGEARUSB_H_

#define MAX_PACKET_SIZE 0x40

//Device Descriptor
#define RG_DD_bLength 0x12
#define RG_DD_bDescriptorType 0x01
#define RG_DD_bcdUSB 0x0200
#define RG_DD_bDeviceClass 0xff
#define RG_DD_bDeviceSubClass 0xff
#define RG_DD_bDeviceProtocol 0xff
#define RG_DD_bMaxPacketSize0 0x40
#define RG_DD_idVendor 0x045e
#define RG_DD_idProduct 0x028e
#define RG_DD_bcdDevice 0x0110
#define RG_DD_iManufacturer 0x01
#define RG_DD_iProduct 0x02
#define RG_DD_iSerialNumber 0x00
#define RG_DD_bNumConfigurations 0x01

//Configuration Descriptor
#define RD_CD_bLength 0x09
#define RD_CD_bDescriptorType 0x02
#define RD_CD_wTotalLength 0x0030
#define RD_CD_bNumInterfaces 0x01
//used by SET_CONFIGURATION to select this configuration:
#define RD_CD_bConfigurationValue 0x01
#define RD_CD_iConfiguration 0x00
#define RD_CD_bmAttributes 0x80
#define RD_CD_bMaxPower 0xfa

//Interface Descriptor
#define RD_ID_bLength 0x09
#define RD_ID_bDescriptorType 0x04
#define RD_ID_bInterfaceNumber 0x00
#define RD_ID_bAlternateSetting 0x00
#define RD_ID_bNumEndpoints 0x02
#define RD_ID_bInterfaceClass 0xff
#define RD_ID_bInterfaceSubClass 0x5d
#define RD_ID_bInterfaceProtocol 0x01
#define RD_ID_iInterface 0x00

//Endpoint Descriptor 1
#define RD_EP1_bLength 0x07
#define RD_EP1_bDescriptorType 0x05
#define RD_EP1_bEndpointAddress 0x81
#define RD_EP1_bmAttributes 0x03
#define RD_EP1_wMaxPacketSize 0x20
#define RD_EP1_bInterval 0x04

//Endpoint Descriptor 2
#define RD_EP2_bLength 0x07
#define RD_EP2_bDescriptorType 0x05
#define RD_EP2_bEndpointAddress 0x02
#define RD_EP2_bmAttributes 0x03
#define RD_EP2_wMaxPacketSize 0x20
#define RD_EP2_bInterval 0x08

//HID report Descriptor
//TODO: fill this after getting HID report descriptor

#endif /* INC_REDGEARUSB_H_ */
