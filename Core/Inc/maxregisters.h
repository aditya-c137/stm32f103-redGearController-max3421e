/*
 * maxregisters.h
 *
 *  Created on: Mar 15, 2021
 *      Author: adity
 */

#ifndef INC_MAXREGISTERS_H_
#define INC_MAXREGISTERS_H_

//MAX host mode registers
#define rRCVFIFO 0x08  //1<<3 r1
#define rSNDFIFO 0x10  //2<<3 r2
#define rSUDFIFO 0x20  //4<<3 r4
#define rRCVBC 0x30    //6<<3 r6
#define rSNDBC 0x38    //7<<3 r7

#define rUSBIRQ 0x68   //0d<<3 r13
//USBIRE bits
#define bmVBUSIRQ 0x40
#define bmNOVBUSIRQ 0x20
#define bmOSCOKIRQ 0x01

#define rUSBIEN 0x70   //0e<<3 r14
//USBEN bits
#define bmVBUSIE 0x40
#define bmNOVBUSIE 0x20
#define bmOSCOKIE 0x01

#define rUSBCTL 0x78   //0f<<3 r15
//USBCTL bits
#define bmCHIPRES 0x20
#define bmPWRDOWN 0x10

#define rCPUCTL 0x80   //10<<3 r16
//CPUCTL bits
#define bmPULSEWID0 0x80
#define bmPULSEWID1 0x40
#define bmIE 0x01

#define rPINCTL 0x88   //11<<3 r17
//PINCTL bits
#define bmFDUPSPI  0x10 //00010000
#define bmINTLEVEL 0x08 //00001000
#define bmPOSINT   0x04 //00000100
#define bmGPXB     0x02 //00000010
#define bmGPXA     0x01 //00000001

#define rREVISION 0x90 //12<<3 r18

#define rIOPINS1 0xa0  //14<<3 r20
//IOPINS1 bits
#define bmGPIN3 0x80
#define bmGPIN2 0x40
#define bmGPIN1 0x20
#define bmGPIN0 0x10
#define bmGPOUT3 0x08
#define bmGPOUT2 0x04
#define bmGPOUT1 0x02
#define bmGPOUT0 0x01

#define rIOPINS2 0xa8  //15<<3 r21
//IOPINS2 bits
#define bmGPIN7 0x80
#define bmGPIN6 0x40
#define bmGPIN5 0x20
#define bmGPIN4 0x10
#define bmGPOUT7 0x08
#define bmGPOUT6 0x04
#define bmGPOUT5 0x02
#define bmGPOUT4 0x01

#define rGPINIRQ 0xb0  //16<<3 r22
//GPINIRQ bits
#define bmGPINIRQ7 0x80
#define bmGPINIRQ6 0x40
#define bmGPINIRQ5 0x20
#define bmGPINIRQ4 0x10
#define bmGPINIRQ3 0x08
#define bmGPINIRQ2 0x04
#define bmGPINIRQ1 0x02
#define bmGPINIRQ0 0x01

#define rGPINIEN 0xb8  //17<<3 r23
//GPINIEN bits
#define bmGPINIEN7 0x80
#define bmGPINIEN6 0x40
#define bmGPINIEN5 0x20
#define bmGPINIEN4 0x10
#define bmGPINIEN3 0x08
#define bmGPINIEN2 0x04
#define bmGPINIEN1 0x02
#define bmGPINIEN0 0x01

#define rGPINPOL 0xc0  //18<<3 r24
//GPINPOL bits
#define bmGPINPOL7 0x80
#define bmGPINPOL6 0x40
#define bmGPINPOL5 0x20
#define bmGPINPOL4 0x10
#define bmGPINPOL3 0x08
#define bmGPINPOL2 0x04
#define bmGPINPOL1 0x02
#define bmGPINPOL0 0x01

#define rHIRQ 0xc8     //19<<3 r25
//HIRQ bits
#define bmHXFRNDIRQ 0x80
#define bmFRAMEIRQ 0x40
#define bmCONDENTIRQ 0x20
#define bmSUSDNIRQ 0x10
#define bmSNDBAVIRQ 0x08
#define bmRCVDAVIRQ 0x04
#define bmRWUIRQ 0x02
#define bmBUSEVENTIRQ 0x01

#define rHIEN 0xd0     //1a<<3 r26
//HIEN bits
#define bmHXFRNDIE 0x80
#define bmFRAMEIE 0x40
#define bmCONDENTIE 0x20
#define bmSUSDNIE 0x10
#define bmSNDBAVIE 0x08
#define bmRCVDAVIE 0x04
#define bmRWUIE 0x02
#define bmBUSEVENTIE 0x01

#define rMODE 0xd8     //1b<<3 r27
//MODE bits
#define bmDPPULLDN 0x80
#define bmDMPULLDN 0x40
#define bmDELAYISO 0x20
#define bmSEPIRQ 0x10
#define bmSOFKAENAB 0x08
#define bmHUBPRE 0x04
#define bmLOWSPEED 0x02
#define bmHOST 0x01

#define rPERADDR 0xe0  //1c<<3 r28

#define rHCTL 0xe8     //1d<<3 r29
//HCTL bits
#define bmSNDTOG1       0x80
#define bmSNDTOG0       0x40
#define bmRCVTOG1       0x20
#define bmRCVTOG0       0x10
#define bmSIGRSM        0x08
#define bmSAMPLEBUS     0x04
#define bmFRMRST        0x02
#define bmBUSRST        0x01

#define rHXFR 0xf0     //1e<<3 r30
//Token configs based on 7:4 study it

#define rHRSL 0xf8     //1f<<3 r31
//HRSL bits
/* HRSL Bits */
#define bmRCVTOGRD  0x10
#define bmSNDTOGRD  0x20
#define bmKSTATUS   0x40
#define bmJSTATUS   0x80
#define bmSE0       0x00    //SE0 - disconnect state
#define bmSE1       0xc0    //SE1 - illegal state

#endif /* INC_MAXRESGISTERS_H_ */
