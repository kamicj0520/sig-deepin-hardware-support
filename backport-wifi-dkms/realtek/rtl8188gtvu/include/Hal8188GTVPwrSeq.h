/******************************************************************************
 *
 * Copyright(c) 2016 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#ifndef REALTEK_POWER_SEQUENCE_8188GTV
#define REALTEK_POWER_SEQUENCE_8188GTV

#include "HalPwrSeqCmd.h"

/*
	Check document WM-20130815-JackieLau-RTL8188GTV_Power_Architecture v08.vsd
	There are 6 HW Power States:
	0: POFF--Power Off
	1: PDN--Power Down
	2: CARDEMU--Card Emulation
	3: ACT--Active Mode
	4: LPS--Low Power State
	5: SUS--Suspend

	The transision from different states are defined below
	TRANS_CARDEMU_TO_ACT
	TRANS_ACT_TO_CARDEMU
	TRANS_CARDEMU_TO_SUS
	TRANS_SUS_TO_CARDEMU
	TRANS_CARDEMU_TO_PDN
	TRANS_ACT_TO_LPS
	TRANS_LPS_TO_ACT

	TRANS_END
*/
#define	RTL8188GTV_TRANS_CARDEMU_TO_ACT_STEPS	13
#define	RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS	15
#define	RTL8188GTV_TRANS_CARDEMU_TO_SUS_STEPS	14
#define	RTL8188GTV_TRANS_SUS_TO_CARDEMU_STEPS	15
#define	RTL8188GTV_TRANS_CARDEMU_TO_PDN_STEPS	15
#define	RTL8188GTV_TRANS_PDN_TO_CARDEMU_STEPS	15
#define	RTL8188GTV_TRANS_ACT_TO_LPS_STEPS		11
#define	RTL8188GTV_TRANS_LPS_TO_ACT_STEPS		13
#define	RTL8188GTV_TRANS_ACT_TO_SWLPS_STEPS		21
#define	RTL8188GTV_TRANS_SWLPS_TO_ACT_STEPS		14
#define	RTL8188GTV_TRANS_END_STEPS		1


#define RTL8188GTV_TRANS_CARDEMU_TO_ACT														\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT2, 0},/* disable SW LPS 0x04[10]=0*/	\
	{0x0006, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT1, BIT1},/* wait till 0x04[17] = 1    power ready*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, 0},/* disable HWPDN 0x04[15]=0*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, (BIT3), 0},/*  0x4[11]=1'b0 disable WL suspend*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0},/* 0x4[8]=1 polling until return 0*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT0, 0},/**/	 \
	{0x0027, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x35}, /*0x27<=35 to reduce RF noise*/

#define RTL8188GTV_TRANS_ACT_TO_CARDEMU													\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x001F, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0},/*0x1F[7:0] = 0 turn off RF*/	\
	{0x004E, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, 0},/*0x4C[23] = 0x4E[7] = 0, switch DPDT_SEL_P output from register 0x65[2] */\
	{0x0027, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x34}, /*0x27 <= 34, xtal_qsel = 0 to xtal bring up*/\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, BIT1}, /*0x04[9] = 1 turn off MAC by HW state machine*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT1, 0}, /*wait till 0x04[9] = 0 polling until return 0 to disable*/	\

#define RTL8188GTV_TRANS_CARDEMU_TO_SUS													\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0007, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x00}, /*0x07 = 0x00 , SOP option to disable BG/MB*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK | PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT3 | BIT4, BIT3}, /*0x04[12:11] = 2b'01 enable WL suspend*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_WRITE, BIT0, BIT0}, /*Set SDIO suspend local register*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_POLLING, BIT1, 0}, /*wait power state to suspend*/ \
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT4, BIT4},/* 0xC4[4] <= 1, turn off USB APHY LDO under suspend mode*/

#define RTL8188GTV_TRANS_SUS_TO_CARDEMU													\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_WRITE, BIT0, 0}, /*Set SDIO suspend local register*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_POLLING, BIT1, BIT1}, /*wait power state to suspend*/\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT3 | BIT4, 0}, /*0x04[12:11] = 2b'01enable WL suspend*/	\
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT4, 0},/* 0xC4[4] <= 1, turn off USB APHY LDO under suspend mode*/

#define RTL8188GTV_TRANS_CARDEMU_TO_CARDDIS													\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0007, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x00}, /*0x07 = 0x00 , SOP option to disable BG/MB*/	\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK | PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT3 | BIT4, BIT3}, /*0x04[12:11] = 2b'01 enable WL suspend*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_WRITE, BIT0, BIT0}, /*Set SDIO suspend local register*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_POLLING, BIT1, 0}, /*wait power state to suspend*/ \
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT4, BIT4},/* 0xC4[4] <= 1, turn off USB APHY LDO under suspend mode*/

#define RTL8188GTV_TRANS_CARDDIS_TO_CARDEMU													\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_WRITE, BIT0, 0}, /*Set SDIO suspend local register*/	\
	{0x0086, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_POLLING, BIT1, BIT1}, /*wait power state to suspend*/\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT3 | BIT4, 0}, /*0x04[12:11] = 2b'01enable WL suspend*/	\
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT4, 0},/* 0xC4[4] <= 1, turn off USB APHY LDO under suspend mode*/


#define RTL8188GTV_TRANS_CARDEMU_TO_PDN												\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0006, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, 0},/* 0x04[16] = 0*/\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, BIT7},/* 0x04[15] = 1*/

#define RTL8188GTV_TRANS_PDN_TO_CARDEMU												\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0005, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, 0},/* 0x04[15] = 0*/

#define RTL8188GTV_TRANS_ACT_TO_LPS														\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0139, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0},/*set RPWM IMR*/	\
	{0x0522, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0xFF},/*Tx Pause*/	\
	{0x05F8, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x05F9, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x05FA, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, 0},/*CCK and OFDM are disabled, and clock are gated*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_DELAY, 0, PWRSEQ_DELAY_US},/*Delay 1us*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, 0},/*Whole BB is reset*/	\
	{0x0100, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x3F},/*Reset MAC TRX*/	\
	{0x0101, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, 0},/*check if removed later*/	\
	{0x0553, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT5, BIT5},/*Respond TxOK to scheduler*/


#define RTL8188GTV_TRANS_LPS_TO_ACT															\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0080, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_SDIO, PWR_CMD_WRITE, 0xFF, 0x84},  /*SDIO RPWM*/\
	{0xFE58, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x84}, /*USB RPWM*/\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_DELAY, 0, PWRSEQ_DELAY_MS}, /*Delay*/\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_DELAY, 0, PWRSEQ_DELAY_MS}, /*Delay*/\
	{0x0027, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x35},/*xtal_qsel = 1 for low noise*/	\
	{0x0109, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT7, 0}, /*Polling 0x109[7]=0  TSF in 40M*/\
	{0x002B, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0x1c, 0x1c},   /*.	0x2b[4:2] = 3b'111	to enable BB, AFE clock*/\
	{0x0101, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, BIT1}, /*.	0x101[1] = 1*/\
	{0x0100, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0xFF}, /*.	0x100[7:0] = 0xFF	 enable WMAC TRX*/\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1 | BIT0, BIT1 | BIT0},  /*.	0x02[1:0] = 2b'11	 enable BB macro*/\
	{0x0522, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0}, /*.	0x522 = 0*/


#define RTL8188GTV_TRANS_ACT_TO_SWLPS														\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0139, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0},/*set RPWM IMR*/	\
	{0x0522, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0xFF},/*Tx Pause*/	\
	{0x05F8, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x05F9, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x05FA, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, 0xFF, 0},/*Should be zero if no packet is transmitting*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, 0},/*CCK and OFDM are disabled, and clock are gated*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_DELAY, 0, PWRSEQ_DELAY_US},/*Delay 1us*/	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, 0},/*Whole BB is reset*/	\
	{0x0100, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x3F},/*Reset MAC TRX*/	\
	{0x0101, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, 0},/*check if removed later*/	\
	{0x0553, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT5, BIT5},/*Respond TxOK to scheduler*/	\
	{0x002b, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0x1C, 0x00},/*0x2b[4:2]<=0 to gated BB, AFE clock*/	\
	{0x0027, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x34},/*xtal_qsel = 0 for bring up*/	\
	{0x0093, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x00},/* sdio LPS option*/	\
	{0x0093, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0x83},/* usb LPS option, open bandgap, xtal*/	\
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT5, 0}, /* 0xC4[5]<=0, digital LDO no standby mode*/	\
	{0x00C4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, BIT7}, /* 0xC4[7]<=1, on domain voltage adjust*/	\
	{0x00a7, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0xe0}, /* low power LPS enable for sdio*/	\
	{0x00a7, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_USB_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xff, 0xe4}, /* low power LPS enable for usb*/	\
	{0x0090, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0}, /* enable WL_LPS_EN*/


#define RTL8188GTV_TRANS_SWLPS_TO_ACT															\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0x0109, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT7, 0},/*polling TSF stable*/\
	{0x0101, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, BIT1}, /*.	0x101[1] = 1, enable security engine*/\
	{0x0100, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0xFF}, /*.	0x100[7:0] = 0xFF	 enable WMAC TRX*/\
	{0x06B7, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x09}, /*.	reset MAC rx state machine*/\
	{0x06B4, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0x86}, /*.	reset MAC rx state machine*/\
	{0x0080, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT1, BIT1},/* set CPU RAM code ready*/	\
	{0x001D, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, 0},/*Reset CPU IO Wrapper*/	\
	{0x0003, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT2, 0},/* Enable CPU*/	\
	{0x001D, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0},/*enable CPU IO Wrapper*/	\
	{0x0003, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT2, BIT2},/* Enable CPU*/	\
	{0x0080, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT7, BIT7},/*polling FW init ready */	\
	{0x0080, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_POLLING, BIT6, BIT6},/*polling FW init ready */	\
	{0x0002, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, BIT0, BIT0}, /*.	0x02[1:0] = 2b'11	 enable BB macro*/\
	{0x0522, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_SDIO_MSK, PWR_BASEADDR_MAC, PWR_CMD_WRITE, 0xFF, 0}, /*.	0x522 = 0*/

#define RTL8188GTV_TRANS_END															\
	/* format */																\
	/* { offset, cut_msk, fab_msk|interface_msk, base|cmd, msk, value }, // comments here*/								\
	{0xFFFF, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_ALL_MSK, 0, PWR_CMD_END, 0, 0},


	extern WLAN_PWR_CFG rtl8188GTV_power_on_flow[RTL8188GTV_TRANS_CARDEMU_TO_ACT_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_radio_off_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_card_disable_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_CARDEMU_TO_PDN_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_card_enable_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_CARDEMU_TO_PDN_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_suspend_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_CARDEMU_TO_SUS_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_resume_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_CARDEMU_TO_SUS_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_hwpdn_flow[RTL8188GTV_TRANS_ACT_TO_CARDEMU_STEPS + RTL8188GTV_TRANS_CARDEMU_TO_PDN_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_enter_lps_flow[RTL8188GTV_TRANS_ACT_TO_LPS_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_leave_lps_flow[RTL8188GTV_TRANS_LPS_TO_ACT_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_enter_swlps_flow[RTL8188GTV_TRANS_ACT_TO_SWLPS_STEPS + RTL8188GTV_TRANS_END_STEPS];
	extern WLAN_PWR_CFG rtl8188GTV_leave_swlps_flow[RTL8188GTV_TRANS_SWLPS_TO_ACT_STEPS + RTL8188GTV_TRANS_END_STEPS];
#endif
