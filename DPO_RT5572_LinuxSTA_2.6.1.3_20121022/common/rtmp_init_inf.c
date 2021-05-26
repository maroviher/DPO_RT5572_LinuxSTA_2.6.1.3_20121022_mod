/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2010, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************/


#include	"rt_config.h"
#include	<rt_led.h>


#ifdef CONFIG_STA_SUPPORT
#ifdef PROFILE_STORE
NDIS_STATUS WriteDatThread(
	IN  RTMP_ADAPTER *pAd);
#endif /* PROFILE_STORE */
#endif /* CONFIG_STA_SUPPORT */

#ifdef LINUX
#ifdef OS_ABL_FUNC_SUPPORT
/* Utilities provided from NET module */
RTMP_NET_ABL_OPS RtmpDrvNetOps, *pRtmpDrvNetOps = &RtmpDrvNetOps;
RTMP_PCI_CONFIG RtmpPciConfig, *pRtmpPciConfig = &RtmpPciConfig;
RTMP_USB_CONFIG RtmpUsbConfig, *pRtmpUsbConfig = &RtmpUsbConfig;

VOID RtmpDrvOpsInit(
	OUT		VOID				*pDrvOpsOrg,
	INOUT	VOID				*pDrvNetOpsOrg,
	IN		RTMP_PCI_CONFIG		*pPciConfig,
	IN		RTMP_USB_CONFIG		*pUsbConfig)
{
	RTMP_DRV_ABL_OPS *pDrvOps = (RTMP_DRV_ABL_OPS *)pDrvOpsOrg;
#ifdef RTMP_USB_SUPPORT
	RTMP_NET_ABL_OPS *pDrvNetOps = (RTMP_NET_ABL_OPS *)pDrvNetOpsOrg;
#endif /* RTMP_USB_SUPPORT */


	/* init PCI/USB configuration in different OS */
	if (pPciConfig != NULL)
		RtmpPciConfig = *pPciConfig;

	if (pUsbConfig != NULL)
		RtmpUsbConfig = *pUsbConfig;

	/* init operators provided from us (DRIVER module) */
	pDrvOps->RTMPAllocAdapterBlock = RTMPAllocAdapterBlock;
	pDrvOps->RTMPFreeAdapter = RTMPFreeAdapter;

	pDrvOps->RtmpRaDevCtrlExit = RtmpRaDevCtrlExit;
	pDrvOps->RtmpRaDevCtrlInit = RtmpRaDevCtrlInit;

	pDrvOps->RTMPSendPackets = RTMPSendPackets;
#ifdef MBSS_SUPPORT
	pDrvOps->MBSS_PacketSend = MBSS_PacketSend;
#endif /* MBSS_SUPPORT */
#ifdef WDS_SUPPORT
	pDrvOps->WDS_PacketSend = WDS_PacketSend;
#endif /* WDS_SUPPORT */
#ifdef APCLI_SUPPORT
	pDrvOps->APC_PacketSend = APC_PacketSend;
#endif /* APCLI_SUPPORT */

	pDrvOps->RTMP_COM_IoctlHandle = RTMP_COM_IoctlHandle;
#ifdef CONFIG_STA_SUPPORT
	pDrvOps->RTMP_STA_IoctlHandle = RTMP_STA_IoctlHandle;
#endif /* CONFIG_STA_SUPPORT */

	pDrvOps->RTMPDrvOpen = RTMPDrvOpen;
	pDrvOps->RTMPDrvClose = RTMPDrvClose;
	pDrvOps->RTMPInfClose = RTMPInfClose;
	pDrvOps->rt28xx_init = rt28xx_init;

	/* init operators provided from us and netif module */
#ifdef RTMP_USB_SUPPORT
	*pRtmpDrvNetOps = *pDrvNetOps;
	pRtmpDrvNetOps->RtmpDrvUsbBulkOutDataPacketComplete = RTUSBBulkOutDataPacketComplete;
	pRtmpDrvNetOps->RtmpDrvUsbBulkOutMLMEPacketComplete = RTUSBBulkOutMLMEPacketComplete;
	pRtmpDrvNetOps->RtmpDrvUsbBulkOutNullFrameComplete = RTUSBBulkOutNullFrameComplete;
#if defined(CONFIG_MULTI_CHANNEL) || defined(DOT11Z_TDLS_SUPPORT)
	pRtmpDrvNetOps->RtmpDrvUsbBulkOutHCCANullFrameComplete = RTUSBBulkOutHCCANullFrameComplete;
#endif /* defined(CONFIG_MULTI_CHANNEL) || defined(DOT11Z_TDLS_SUPPORT) */
/*	pRtmpDrvNetOps->RtmpDrvUsbBulkOutRTSFrameComplete = RTUSBBulkOutRTSFrameComplete;*/
	pRtmpDrvNetOps->RtmpDrvUsbBulkOutPsPollComplete = RTUSBBulkOutPsPollComplete;
	pRtmpDrvNetOps->RtmpDrvUsbBulkRxComplete = RTUSBBulkRxComplete;
	*pDrvNetOps = *pRtmpDrvNetOps;
#endif /* RTMP_USB_SUPPORT */
}

RTMP_BUILD_DRV_OPS_FUNCTION_BODY

#endif /* OS_ABL_FUNC_SUPPORT */
#endif /* LINUX */

#ifndef LED_CONTROL_SUPPORT
#ifdef RT3070
void MyRTMPTurnOffLED(IN RTMP_ADAPTER *pAd)
{
	LED_CONTROL my_LED_CONTROL;
	USHORT Value;
	PLED_CONTROL pLedCntl = &my_LED_CONTROL;

	RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, Value);
	pLedCntl->MCULedCntl.word = (Value >> 8);
	RT28xx_EEPROM_READ16(pAd, EEPROM_LEDAG_CONF_OFFSET, Value);
	pLedCntl->LedAGCfg= Value;
	RT28xx_EEPROM_READ16(pAd, EEPROM_LEDACT_CONF_OFFSET, Value);
	pLedCntl->LedACTCfg = Value;
	RT28xx_EEPROM_READ16(pAd, EEPROM_LED_POLARITY_OFFSET, Value);
	pLedCntl->LedPolarity = Value;
	
	UCHAR LedMode = (pLedCntl->MCULedCntl.field.LedMode & 0x7F);
	if (pLedCntl->MCULedCntl.word == 0xFF)
	{
		pLedCntl->MCULedCntl.word = 0x01;
		pLedCntl->LedAGCfg = 0x5555;
		pLedCntl->LedACTCfg= 0x2221;

#ifdef RTMP_MAC_USB
		pLedCntl->LedPolarity = 0x5627; 
#endif /* RTMP_MAC_USB */
	}
	
	AsicSendCommandToMcu(pAd, MCU_SET_LED_AG_CFG, 0xff, (UCHAR)pLedCntl->LedAGCfg, (UCHAR)(pLedCntl->LedAGCfg >> 8), FALSE);
	AsicSendCommandToMcu(pAd, MCU_SET_LED_ACT_CFG, 0xff, (UCHAR)pLedCntl->LedACTCfg, (UCHAR)(pLedCntl->LedACTCfg >> 8), FALSE);
	AsicSendCommandToMcu(pAd, MCU_SET_LED_POLARITY, 0xff, (UCHAR)pLedCntl->LedPolarity, (UCHAR)(pLedCntl->LedPolarity >> 8), FALSE);
	AsicSendCommandToMcu(pAd, MCU_SET_LED_GPIO_SIGNAL_CFG, 0xff, 0, pLedCntl->MCULedCntl.field.Polarity, FALSE);
	AsicSendCommandToMcu(pAd, MCU_SET_LED_MODE, 0xff, LedMode, LINK_STATUS_LINK_DOWN, FALSE);
	AsicSendCommandToMcu(pAd, MCU_SET_LED_MODE, 0xff, 0, 0x00, FALSE);

}
#endif
#endif

int rt28xx_init(
	IN VOID		*pAdSrc,
	IN PSTRING	pDefaultMac, 
	IN PSTRING	pHostName)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	UINT					index;
	UCHAR					TmpPhy;
	NDIS_STATUS				Status;

	if (pAd == NULL)
		return FALSE;

#ifdef RT3290
	DBGPRINT(RT_DEBUG_OFF, ("MACVersion=0x%x\n", pAd->MACVersion));
	if (IS_RT3290(pAd))
	{
		UINT32 MacRegValue;
		OSCCTL_STRUC osCtrl = {.word = 0};
		CMB_CTRL_STRUC cmbCtrl = {.word = 0};
		WLAN_FUN_CTRL_STRUC WlanFunCtrl = {.word = 0};
			
		RTMPEnableWlan(pAd, TRUE, TRUE);

		//
		// Too much time for reading efuse(enter/exit L1), and our device will hang up
		// Disable L1
		//
		RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &WlanFunCtrl.word);
		if (WlanFunCtrl.field.WLAN_EN == TRUE)
		{
			WlanFunCtrl.field.PCIE_APP0_CLK_REQ = TRUE;
			RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
		}
			
		//Enable ROSC_EN first then CAL_REQ
		RTMP_IO_READ32(pAd, OSCCTL, &osCtrl.word);
		osCtrl.field.ROSC_EN = TRUE; //HW force 
		RTMP_IO_WRITE32(pAd, OSCCTL, osCtrl.word);	
		
		osCtrl.field.ROSC_EN = TRUE; //HW force 
		osCtrl.field.CAL_REQ = TRUE;
		osCtrl.field.REF_CYCLE = 0x27;
		RTMP_IO_WRITE32(pAd, OSCCTL, osCtrl.word);

		RTMP_IO_READ32(pAd, CMB_CTRL, &cmbCtrl.word);
		pAd->CmbCtrl.word = cmbCtrl.word;

		// Overwrite default Coex Parameter
		RTMP_IO_READ32(pAd, COEXCFG0, &MacRegValue);
		MacRegValue &= ~(0xFF000000);
		MacRegValue |= 0x5E000000;
		RTMP_IO_WRITE32(pAd, COEXCFG0, MacRegValue);
	}

	if (IS_RT3290LE(pAd))
	{
		PLL_CTRL_STRUC PllCtrl;
		RTMP_IO_READ32(pAd, PLL_CTRL, &PllCtrl.word);
		PllCtrl.field.VCO_FIXED_CURRENT_CONTROL = 0x1;			
		RTMP_IO_WRITE32(pAd, PLL_CTRL, PllCtrl.word);
	}
#endif /* RT3290 */

#ifdef CONFIG_STA_SUPPORT
#ifdef PCIE_PS_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
    	/* If dirver doesn't wake up firmware here,*/
    	/* NICLoadFirmware will hang forever when interface is up again.*/
    	if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_DOZE) &&
        	OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE))
    	{
        	AUTO_WAKEUP_STRUC AutoWakeupCfg;
			AsicForceWakeup(pAd, TRUE);
        	AutoWakeupCfg.word = 0;
	    	RTMP_IO_WRITE32(pAd, AUTO_WAKEUP_CFG, AutoWakeupCfg.word);
        	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_DOZE);
    	}
	}
#endif /* PCIE_PS_SUPPORT */
#endif /* CONFIG_STA_SUPPORT */

	/* reset Adapter flags*/
	RTMP_CLEAR_FLAGS(pAd);

	/* Init BssTab & ChannelInfo tabbles for auto channel select.*/

#ifdef DOT11_N_SUPPORT
	/* Allocate BA Reordering memory*/
	if (ba_reordering_resource_init(pAd, MAX_REORDERING_MPDU_NUM) != TRUE)		
		goto err1;
#endif /* DOT11_N_SUPPORT */

	/* Make sure MAC gets ready.*/
	index = 0;
	if (WaitForAsicReady(pAd) != TRUE)
		goto err1;

	DBGPRINT(RT_DEBUG_TRACE, ("MAC[Ver:Rev=0x%08x]\n", pAd->MACVersion));
		

	if (MAX_LEN_OF_MAC_TABLE > MAX_AVAILABLE_CLIENT_WCID(pAd))
	{
		DBGPRINT(RT_DEBUG_ERROR, ("MAX_LEN_OF_MAC_TABLE can not be larger than MAX_AVAILABLE_CLIENT_WCID!!!!\n"));
		goto err1;
	}


	/* Disable DMA*/
	RT28XXDMADisable(pAd);


	/* Load 8051 firmware*/
	Status = NICLoadFirmware(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("NICLoadFirmware failed, Status[=0x%08x]\n", Status));
		goto err1;
	}

	NICLoadRateSwitchingParams(pAd);

	/* Disable interrupts here which is as soon as possible*/
	/* This statement should never be true. We might consider to remove it later*/

#ifdef RESOURCE_PRE_ALLOC
	Status = RTMPInitTxRxRingMemory(pAd);
#else
	Status = RTMPAllocTxRxRingMemory(pAd);
#endif /* RESOURCE_PRE_ALLOC */

	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("RTMPAllocTxRxMemory failed, Status[=0x%08x]\n", Status));
		goto err2;
	}

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);

	/* initialize MLME*/
	
	Status = RtmpMgmtTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err3;

	Status = MlmeInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("MlmeInit failed, Status[=0x%08x]\n", Status));
		goto err4;
	}

#ifdef RMTP_RBUS_SUPPORT
#ifdef VIDEO_TURBINE_SUPPORT
	VideoConfigInit(pAd);
#endif /* VIDEO_TURBINE_SUPPORT */
#endif /* RMTP_RBUS_SUPPORT */

	/* Initialize pAd->StaCfg, pAd->ApCfg, pAd->CommonCfg to manufacture default*/
	
	UserCfgInit(pAd);


	Status = RtmpNetTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err5;

/*	COPY_MAC_ADDR(pAd->ApCfg.MBSSID[apidx].Bssid, netif->hwaddr);*/
/*	pAd->bForcePrintTX = TRUE;*/

	CfgInitHook(pAd);


#ifdef BLOCK_NET_IF
	initblockQueueTab(pAd);
#endif /* BLOCK_NET_IF */

	Status = MeasureReqTabInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("MeasureReqTabInit failed, Status[=0x%08x]\n",Status));
		goto err6;	
	}
	Status = TpcReqTabInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("TpcReqTabInit failed, Status[=0x%08x]\n",Status));
		goto err6;	
	}

	
	/* Init the hardware, we need to init asic before read registry, otherwise mac register will be reset*/
	
	Status = NICInitializeAdapter(pAd, TRUE);
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("NICInitializeAdapter failed, Status[=0x%08x]\n", Status));
		if (Status != NDIS_STATUS_SUCCESS)
		goto err6;
	}	


	/* Read parameters from Config File */
	/* unknown, it will be updated in NICReadEEPROMParameters */
	pAd->RfIcType = RFIC_UNKNOWN;
	Status = RTMPReadParametersHook(pAd);

#ifdef CONFIG_STA_SUPPORT
#ifdef CREDENTIAL_STORE
	RecoverConnectInfo(pAd);
#endif /* CREDENTIAL_STORE */
#endif /* CONFIG_STA_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("1. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));
	if (Status != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT_ERR(("RTMPReadParametersHook failed, Status[=0x%08x]\n",Status));
		goto err6;
	}

#ifdef RTMP_MAC_USB
	pAd->CommonCfg.bMultipleIRP = FALSE;

	if (pAd->CommonCfg.bMultipleIRP)
		pAd->CommonCfg.NumOfBulkInIRP = RX_RING_SIZE;
	else
		pAd->CommonCfg.NumOfBulkInIRP = 1;
#endif /* RTMP_MAC_USB */

#ifdef DOT11_N_SUPPORT
   	/*Init Ba Capability parameters.*/
/*	RT28XX_BA_INIT(pAd);*/
	pAd->CommonCfg.DesiredHtPhy.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;
	pAd->CommonCfg.DesiredHtPhy.AmsduEnable = (USHORT)pAd->CommonCfg.BACapability.field.AmsduEnable;
	pAd->CommonCfg.DesiredHtPhy.AmsduSize = (USHORT)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.DesiredHtPhy.MimoPs = (USHORT)pAd->CommonCfg.BACapability.field.MMPSmode;
	/* UPdata to HT IE*/
	pAd->CommonCfg.HtCapability.HtCapInfo.MimoPs = (USHORT)pAd->CommonCfg.BACapability.field.MMPSmode;
	pAd->CommonCfg.HtCapability.HtCapInfo.AMsduSize = (USHORT)pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.HtCapability.HtCapParm.MpduDensity = (UCHAR)pAd->CommonCfg.BACapability.field.MpduDensity;
#endif /* DOT11_N_SUPPORT */

	/* after reading Registry, we now know if in AP mode or STA mode*/

	/* Load 8051 firmware; crash when FW image not existent*/
	/* Status = NICLoadFirmware(pAd);*/
	/* if (Status != NDIS_STATUS_SUCCESS)*/
	/*    break;*/

	DBGPRINT(RT_DEBUG_TRACE, ("2. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

	/* We should read EEPROM for all cases.  rt2860b*/
	NICReadEEPROMParameters(pAd, (PSTRING)pDefaultMac);	
#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("3. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

#ifdef LED_CONTROL_SUPPORT
	/* Send LED Setting to MCU */
	RTMPInitLEDMode(pAd);
#else
#ifdef RT3070
	//turn off LED if we dont have LED support
	MyRTMPTurnOffLED(pAd);
#endif
#endif /* LED_CONTROL_SUPPORT */

	NICInitAsicFromEEPROM(pAd); /* rt2860b */


#ifdef RTMP_FREQ_CALIBRATION_SUPPORT
#ifdef CONFIG_STA_SUPPORT
	/* Initialize the frequency calibration*/
	if (pAd->chipCap.FreqCalibrationSupport)
		FrequencyCalibration(pAd);	
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_FREQ_CALIBRATION_SUPPORT */

#ifdef RTMP_INTERNAL_TX_ALC
	/* Initialize the desired TSSI table*/
	RTMP_CHIP_ASIC_TSSI_TABLE_INIT(pAd);
#endif /* RTMP_INTERNAL_TX_ALC */

#ifdef RTMP_TEMPERATURE_COMPENSATION
	/* Temperature compensation, initialize the lookup table */
	DBGPRINT(RT_DEBUG_TRACE, ("bAutoTxAgcG = %d\n", pAd->bAutoTxAgcG));

	if (pAd->chipCap.bTempCompTxALC && pAd->bAutoTxAgcG)
		InitLookupTable(pAd);
#endif /* RTMP_TEMPERATURE_COMPENSATION */

#ifdef RTMP_FREQ_CALIBRATION_SUPPORT
#ifdef CONFIG_STA_SUPPORT
	if (pAd->chipCap.FreqCalibrationSupport)
		InitFrequencyCalibration(pAd);
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_FREQ_CALIBRATION_SUPPORT */

	/* Set PHY to appropriate mode*/
	TmpPhy = pAd->CommonCfg.PhyMode;
	pAd->CommonCfg.PhyMode = 0xff;
	RTMPSetPhyMode(pAd, TmpPhy);
#ifdef DOT11_N_SUPPORT
	SetCommonHT(pAd);
#endif /* DOT11_N_SUPPORT */

	/* No valid channels.*/
	if (pAd->ChannelListNum == 0)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Wrong configuration. No valid channel found. Check \"ContryCode\" and \"ChannelGeography\" setting.\n"));
		goto err6;
	}

#ifdef DOT11_N_SUPPORT
	DBGPRINT(RT_DEBUG_TRACE, ("MCS Set = %02x %02x %02x %02x %02x\n", pAd->CommonCfg.HtCapability.MCSSet[0],
           pAd->CommonCfg.HtCapability.MCSSet[1], pAd->CommonCfg.HtCapability.MCSSet[2],
           pAd->CommonCfg.HtCapability.MCSSet[3], pAd->CommonCfg.HtCapability.MCSSet[4]));
#endif /* DOT11_N_SUPPORT */



/*		APInitialize(pAd);*/

#ifdef IKANOS_VX_1X0
	VR_IKANOS_FP_Init(pAd->ApCfg.BssidNum, pAd->PermanentAddress);
#endif /* IKANOS_VX_1X0 */

#ifdef RTMP_MAC_USB
	AsicSendCommandToMcu(pAd, 0x31, 0xff, 0x00, 0x02, FALSE);
	RTMPusecDelay(10000);
#endif /* RTMP_MAC_USB */



	/*
		Some modules init must be called before APStartUp().
		Or APStartUp() will make up beacon content and call
		other modules API to get some information to fill.
	*/



#ifdef CONFIG_TSO_SUPPORT
	if (RTMP_TEST_MORE_FLAG(pAd, fRTMP_ADAPTER_TSO_SUPPORT))
		RTMPTsoEnable(pAd);
#endif /* CONFIG_TSO_SUPPORT */

	if (pAd && (Status != NDIS_STATUS_SUCCESS))
	{
		
		/* Undo everything if it failed*/
		
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
		{
/*			NdisMDeregisterInterrupt(&pAd->Interrupt);*/
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
		}
/*		RTMPFreeAdapter(pAd);  we will free it in disconnect()*/
	}
	else if (pAd)
	{
		/* Microsoft HCT require driver send a disconnect event after driver initialization.*/
		OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
		OPSTATUS_CLEAR_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED);
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MEDIA_STATE_CHANGE);

		DBGPRINT(RT_DEBUG_TRACE, ("NDIS_STATUS_MEDIA_DISCONNECT Event B!\n"));


#ifdef RTMP_MAC_USB
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);

		
		/* Support multiple BulkIn IRP,*/
		/* the value on pAd->CommonCfg.NumOfBulkInIRP may be large than 1.*/
		
		for(index=0; index<pAd->CommonCfg.NumOfBulkInIRP; index++)
		{
			RTUSBBulkReceive(pAd);
			DBGPRINT(RT_DEBUG_TRACE, ("RTUSBBulkReceive!\n" ));
		}
#endif /* RTMP_MAC_USB */
	}/* end of else*/

	/* Set up the Mac address*/
#ifdef CONFIG_STA_SUPPORT
	RtmpOSNetDevAddrSet(pAd->OpMode, pAd->net_dev, &pAd->CurrentAddress[0], (PUCHAR)(pAd->StaCfg.dev_name));
#endif /* CONFIG_STA_SUPPORT */

	/* Various AP function init*/

#ifdef UAPSD_SUPPORT
        UAPSD_Init(pAd);
#endif /* UAPSD_SUPPORT */

	/* assign function pointers*/





#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{

#ifdef WPA_SUPPLICANT_SUPPORT
#ifndef NATIVE_WPA_SUPPLICANT_SUPPORT
		/* send wireless event to wpa_supplicant for infroming interface up.*/
		RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CUSTOM, RT_INTERFACE_UP, NULL, NULL, 0);
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */
#endif /* WPA_SUPPLICANT_SUPPORT */

	}
#endif /* CONFIG_STA_SUPPORT */

	/* auto-fall back settings */
#ifdef RANGE_EXTEND
	RTMP_IO_WRITE32(pAd, HT_FBK_CFG1, 0xedcba980);
#endif // RANGE_EXTEND //
#ifdef DOT11N_SS3_SUPPORT
	if (pAd->CommonCfg.TxStream >= 3)
	{
		RTMP_IO_WRITE32(pAd, TX_FBK_CFG_3S_0, 0x12111008);
		RTMP_IO_WRITE32(pAd, TX_FBK_CFG_3S_1, 0x16151413);
	}
#endif /* DOT11N_SS3_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
	RtmpStreamModeInit(pAd);
#endif /* STREAM_MODE_SUPPORT */


#ifdef DOT11_N_SUPPORT
#endif /* DOT11_N_SUPPORT */



#ifdef RT3290
	if (IS_RT3290(pAd))
	{
		WLAN_FUN_CTRL_STRUC     WlanFunCtrl = {.word = 0};
		RTMP_MAC_PWRSV_EN(pAd, TRUE, TRUE);	
		//
		// Too much time for reading efuse(enter/exit L1), and our device will hang up
		// Enable L1
		//
		RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &WlanFunCtrl.word);
		if (WlanFunCtrl.field.WLAN_EN == TRUE)
		{
			WlanFunCtrl.field.PCIE_APP0_CLK_REQ = FALSE;
			RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, WlanFunCtrl.word);
		}
	}
#endif /* RT3290 */

	return TRUE;

/*err7:
	APStop(pAd);*/
err6:

#ifdef IGMP_SNOOP_SUPPORT
	MultiCastFilterTableReset(&pAd->pMulticastFilterTable);
#endif /* IGMP_SNOOP_SUPPORT */

	MeasureReqTabExit(pAd);
	TpcReqTabExit(pAd);
err5:	
	RtmpNetTaskExit(pAd);
	UserCfgExit(pAd);
err4:	
	MlmeHalt(pAd);
	RTMP_TimerListRelease(pAd);
err3:	
	RtmpMgmtTaskExit(pAd);
#ifdef RTMP_TIMER_TASK_SUPPORT
	NdisFreeSpinLock(&pAd->TimerQLock);
#endif /* RTMP_TIMER_TASK_SUPPORT */
err2:
#ifdef RESOURCE_PRE_ALLOC
	RTMPResetTxRxRingMemory(pAd);
#else
	RTMPFreeTxRxRingMemory(pAd);
#endif /* RESOURCE_PRE_ALLOC */

err1:


#ifdef RT3290
	if (IS_RT3290(pAd))
		RTMPEnableWlan(pAd, FALSE, FALSE);
#endif /* RT3290 */

#ifdef DOT11_N_SUPPORT
	if(pAd->mpdu_blk_pool.mem)
		os_free_mem(pAd, pAd->mpdu_blk_pool.mem); /* free BA pool*/
#endif /* DOT11_N_SUPPORT */

	/* shall not set priv to NULL here because the priv didn't been free yet.*/
	/*net_dev->priv = 0;*/
#ifdef INF_AMAZON_SE
err0:
#endif /* INF_AMAZON_SE */
#ifdef ST
err0:
#endif /* ST */

	DBGPRINT(RT_DEBUG_ERROR, ("!!! rt28xx Initialized fail !!!\n"));
	return FALSE;
}


VOID RTMPDrvOpen(
	IN VOID			*pAdSrc)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)pAdSrc;

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

	/* Enable Interrupt*/
	RTMP_IRQ_ENABLE(pAd);

	/* Now Enable RxTx*/
	RTMPEnableRxTx(pAd);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);

	{
	UINT32 reg = 0;
	RTMP_IO_READ32(pAd, 0x1300, &reg);  /* clear garbage interrupts*/
	DBGPRINT(RT_DEBUG_TRACE, ("0x1300 = %08x\n", reg));
	}

	{
/*	u32 reg;*/
/*	UINT8  byte;*/
/*	u16 tmp;*/

/*	RTMP_IO_READ32(pAd, XIFS_TIME_CFG, &reg);*/

/*	tmp = 0x0805;*/
/*	reg  = (reg & 0xffff0000) | tmp;*/
/*	RTMP_IO_WRITE32(pAd, XIFS_TIME_CFG, reg);*/

	}


#ifdef CONFIG_STA_SUPPORT
#ifdef PCIE_PS_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
        RTMPInitPCIeLinkCtrlValue(pAd);
#endif /* PCIE_PS_SUPPORT */


#endif /* CONFIG_STA_SUPPORT */



#ifdef CONFIG_STA_SUPPORT
	/*
		To reduce connection time, 
		do auto reconnect here instead of waiting STAMlmePeriodicExec to do auto reconnect.
	*/
	if (pAd->OpMode == OPMODE_STA)
		MlmeAutoReconnectLastSSID(pAd);
#endif /* CONFIG_STA_SUPPORT */


#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */


#ifdef CONFIG_MULTI_CHANNEL
	MultiChannelThreadInit(pAd);
#endif /* CONFIG_MULTI_CHANNEL */

}


VOID RTMPDrvClose(
	IN VOID				*pAdSrc,
	IN VOID				*net_dev)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)pAdSrc;
	BOOLEAN 		Cancelled;
	UINT32			i = 0;


	Cancelled = FALSE;

#ifdef CONFIG_STA_SUPPORT
#ifdef CREDENTIAL_STORE
		if (pAd->IndicateMediaState == NdisMediaStateConnected)
		{	
			StoreConnectInfo(pAd);
		}
		else
		{
			RTMP_SEM_LOCK(&pAd->StaCtIf.Lock);
			pAd->StaCtIf.Changeable = FALSE;
			RTMP_SEM_UNLOCK(&pAd->StaCtIf.Lock);
		}
#endif /* CREDENTIAL_STORE */
#endif /* CONFIG_STA_SUPPORT */



#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
#ifdef PCIE_PS_SUPPORT
		RTMPPCIeLinkCtrlValueRestore(pAd, RESTORE_CLOSE);
#endif /* PCIE_PS_SUPPORT */

		/* If dirver doesn't wake up firmware here,*/
		/* NICLoadFirmware will hang forever when interface is up again.*/
		if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_DOZE))
        {      
		    AsicForceWakeup(pAd, TRUE);
        }

#ifdef RTMP_MAC_USB
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);
#endif /* RTMP_MAC_USB */

	}
#endif /* CONFIG_STA_SUPPORT */

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

#ifdef EXT_BUILD_CHANNEL_LIST
	if (pAd->CommonCfg.pChDesp != NULL)
		os_free_mem(NULL, pAd->CommonCfg.pChDesp);
	pAd->CommonCfg.pChDesp = NULL;
	pAd->CommonCfg.DfsType = MAX_RD_REGION;
	pAd->CommonCfg.bCountryFlag = 0;
#endif /* EXT_BUILD_CHANNEL_LIST */



#ifdef WDS_SUPPORT
	WdsDown(pAd);
#endif /* WDS_SUPPORT */

	for (i = 0 ; i < NUM_OF_TX_RING; i++)
	{
		while (pAd->DeQueueRunning[i] == TRUE)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("Waiting for TxQueue[%d] done..........\n", i));
			RTMPusecDelay(1000);
		}
	}
	
#ifdef RTMP_MAC_USB
	RtmpOsUsbEmptyUrbCheck(&pAd->wait, &pAd->BulkInLock, &pAd->PendingRx);

#endif /* RTMP_MAC_USB */


	/* Stop Mlme state machine*/
	MlmeHalt(pAd);
	
	/* Close net tasklets*/
	RtmpNetTaskExit(pAd);


#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		MacTableReset(pAd);
#if defined(WOW_SUPPORT) && defined(RTMP_MAC_USB) && defined(WOW_IFDOWN_SUPPORT)
		if (pAd->WOW_Cfg.bEnable == TRUE)
			RT28xxUsbAsicWOWEnable(pAd);
		else
#endif /* WOW_SUPPORT */
			MlmeRadioOff(pAd);
	}
#endif /* CONFIG_STA_SUPPORT */


	MeasureReqTabExit(pAd);
	TpcReqTabExit(pAd);

#ifdef LED_CONTROL_SUPPORT
	RTMPExitLEDMode(pAd);
#endif // LED_CONTROL_SUPPORT


	/* Close kernel threads*/
	RtmpMgmtTaskExit(pAd);



	/* Free IRQ*/
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
	{
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
	}

	/* Free Ring or USB buffers*/
#ifdef RESOURCE_PRE_ALLOC
	RTMPResetTxRxRingMemory(pAd);
#else
	/* Free Ring or USB buffers*/
	RTMPFreeTxRxRingMemory(pAd);
#endif /* RESOURCE_PRE_ALLOC */

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

#ifdef DOT11_N_SUPPORT
	/* Free BA reorder resource*/
	ba_reordering_resource_release(pAd);
#endif /* DOT11_N_SUPPORT */

	UserCfgExit(pAd); /* must after ba_reordering_resource_release */

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_START_UP);

/*+++Modify by woody to solve the bulk fail+++*/
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
#ifdef RT35xx
		if (IS_RT3572(pAd))
		{
			RT30xxWriteRFRegister(pAd, RF_R08, 0x00);
			AsicSendCommandToMcu(pAd, 0x30, 0xff, 0xff, 0x02, FALSE);
		}
#endif /* RT35xx */
	}
#endif /* CONFIG_STA_SUPPORT */

	/* clear MAC table */
	/* TODO: do not clear spin lock, such as fLastChangeAccordingMfbLock */
	NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));

	/* release all timers */
	RTMPusecDelay(2000);
	RTMP_TimerListRelease(pAd);

#ifdef CONFIG_MULTI_CHANNEL
	MultiChannelThreadExit(pAd);
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef RTMP_TIMER_TASK_SUPPORT
	NdisFreeSpinLock(&pAd->TimerQLock);
#endif /* RTMP_TIMER_TASK_SUPPORT */
}


VOID RTMPInfClose(
	IN VOID				*pAdSrc)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)pAdSrc;





#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
#ifdef PROFILE_STORE
		WriteDatThread(pAd);
		RTMPusecDelay(1000);
#endif /* PROFILE_STORE */
#ifdef QOS_DLS_SUPPORT
		/* send DLS-TEAR_DOWN message, */
		if (pAd->CommonCfg.bDLSCapable)
		{
			UCHAR i;

			/* tear down local dls table entry*/
			for (i=0; i<MAX_NUM_OF_INIT_DLS_ENTRY; i++)
			{
				if (pAd->StaCfg.DLSEntry[i].Valid && (pAd->StaCfg.DLSEntry[i].Status == DLS_FINISH))
				{
					RTMPSendDLSTearDownFrame(pAd, pAd->StaCfg.DLSEntry[i].MacAddr);
					pAd->StaCfg.DLSEntry[i].Status	= DLS_NONE;
					pAd->StaCfg.DLSEntry[i].Valid	= FALSE;
				}
			}

			/* tear down peer dls table entry*/
			for (i=MAX_NUM_OF_INIT_DLS_ENTRY; i<MAX_NUM_OF_DLS_ENTRY; i++)
			{
				if (pAd->StaCfg.DLSEntry[i].Valid && (pAd->StaCfg.DLSEntry[i].Status == DLS_FINISH))
				{
					RTMPSendDLSTearDownFrame(pAd, pAd->StaCfg.DLSEntry[i].MacAddr);
					pAd->StaCfg.DLSEntry[i].Status = DLS_NONE;
					pAd->StaCfg.DLSEntry[i].Valid	= FALSE;
				}
			}
			RTMP_MLME_HANDLER(pAd);
		}
#endif /* QOS_DLS_SUPPORT */

		if (INFRA_ON(pAd) &&
#if defined(WOW_SUPPORT) && defined(RTMP_MAC_USB) && defined(WOW_IFDOWN_SUPPORT) /* In WOW state, can't issue disassociation reqeust */
			pAd->WOW_Cfg.bEnable == FALSE &&
#endif /* WOW_SUPPORT */
			(!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)))
		{
			MLME_DISASSOC_REQ_STRUCT	DisReq;
			MLME_QUEUE_ELEM *MsgElem;/* = (MLME_QUEUE_ELEM *) kmalloc(sizeof(MLME_QUEUE_ELEM), MEM_ALLOC_FLAG);*/
    
			os_alloc_mem(NULL, (UCHAR **)&MsgElem, sizeof(MLME_QUEUE_ELEM));
			if (MsgElem)
			{
			COPY_MAC_ADDR(DisReq.Addr, pAd->CommonCfg.Bssid);
			DisReq.Reason =  REASON_DEAUTH_STA_LEAVING;

			MsgElem->Machine = ASSOC_STATE_MACHINE;
			MsgElem->MsgType = MT2_MLME_DISASSOC_REQ;
			MsgElem->MsgLen = sizeof(MLME_DISASSOC_REQ_STRUCT);
			NdisMoveMemory(MsgElem->Msg, &DisReq, sizeof(MLME_DISASSOC_REQ_STRUCT));

			/* Prevent to connect AP again in STAMlmePeriodicExec*/
			pAd->MlmeAux.AutoReconnectSsidLen= 32;
			NdisZeroMemory(pAd->MlmeAux.AutoReconnectSsid, pAd->MlmeAux.AutoReconnectSsidLen);

			pAd->Mlme.CntlMachine.CurrState = CNTL_WAIT_OID_DISASSOC;
			MlmeDisassocReqAction(pAd, MsgElem);
/*			kfree(MsgElem);*/
			os_free_mem(NULL, MsgElem);
			}
			
			RTMPusecDelay(1000);
		}

#ifdef WPA_SUPPLICANT_SUPPORT
#ifndef NATIVE_WPA_SUPPLICANT_SUPPORT
		/* send wireless event to wpa_supplicant for infroming interface down.*/
		RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CUSTOM, RT_INTERFACE_DOWN, NULL, NULL, 0);
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */

		if (pAd->StaCfg.pWpsProbeReqIe)
		{
/*			kfree(pAd->StaCfg.pWpsProbeReqIe);*/
			os_free_mem(NULL, pAd->StaCfg.pWpsProbeReqIe);
			pAd->StaCfg.pWpsProbeReqIe = NULL;
			pAd->StaCfg.WpsProbeReqIeLen = 0;
		}

		if (pAd->StaCfg.pWpaAssocIe)
		{
/*			kfree(pAd->StaCfg.pWpaAssocIe);*/
			os_free_mem(NULL, pAd->StaCfg.pWpaAssocIe);
			pAd->StaCfg.pWpaAssocIe = NULL;
			pAd->StaCfg.WpaAssocIeLen = 0;
		}
#endif /* WPA_SUPPLICANT_SUPPORT */


	}
#endif /* CONFIG_STA_SUPPORT */
}




PNET_DEV RtmpPhyNetDevMainCreate(
	IN VOID				*pAdSrc)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdSrc;
	PNET_DEV pDevNew;
	UINT32 MC_RowID = 0, IoctlIF = 0;


	pAd = pAd;

#ifdef MULTIPLE_CARD_SUPPORT
	MC_RowID = pAd->MC_RowID;
#endif /* MULTIPLE_CARD_SUPPORT */
#ifdef HOSTAPD_SUPPORT
	IoctlIF = pAd->IoctlIF;
#endif /* HOSTAPD_SUPPORT */

	pDevNew = RtmpOSNetDevCreate((INT32)MC_RowID, (UINT32 *)&IoctlIF,
					INT_MAIN, 0, sizeof(PRTMP_ADAPTER), INF_MAIN_DEV_NAME);

#ifdef HOSTAPD_SUPPORT
	pAd->IoctlIF = IoctlIF;
#endif /* HOSTAPD_SUPPORT */

	return pDevNew;
}

#ifdef CONFIG_STA_SUPPORT
#ifdef PROFILE_STORE
static void	WriteConfToDatFile(
    IN  PRTMP_ADAPTER pAd)
{
	char	*cfgData = 0;
	PSTRING			fileName = NULL;
	RTMP_OS_FD		file_r, file_w;
	RTMP_OS_FS_INFO		osFSInfo;
	LONG			rv, fileLen = 0;
	char			*offset = 0;
	PSTRING			pTempStr = 0;
//	INT				tempStrLen = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("-----> WriteConfToDatFile\n"));

		fileName = STA_PROFILE_PATH;

	RtmpOSFSInfoChange(&osFSInfo, TRUE);

	file_r = RtmpOSFileOpen(fileName, O_RDONLY, 0);
	if (IS_FILE_OPEN_ERR(file_r)) 
	{
		DBGPRINT(RT_DEBUG_TRACE, ("-->1) %s: Error opening file %s\n", __FUNCTION__, fileName));
		return;
	}
	else 
	{
		char tempStr[64] = {0};
		while((rv = RtmpOSFileRead(file_r, tempStr, 64)) > 0)
		{
			fileLen += rv;
		}
		os_alloc_mem(NULL, (UCHAR **)&cfgData, fileLen);
		if (cfgData == NULL)
		{
			RtmpOSFileClose(file_r);
			DBGPRINT(RT_DEBUG_TRACE, ("CfgData kmalloc fail. (fileLen = %ld)\n", fileLen));
			goto out;
		}
		NdisZeroMemory(cfgData, fileLen);
		RtmpOSFileSeek(file_r, 0);
		rv = RtmpOSFileRead(file_r, (PSTRING)cfgData, fileLen);
		RtmpOSFileClose(file_r);
		if (rv != fileLen)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("CfgData kmalloc fail, fileLen = %ld\n", fileLen));
			goto ReadErr;
		}
	}

	file_w = RtmpOSFileOpen(fileName, O_WRONLY|O_TRUNC, 0);
	if (IS_FILE_OPEN_ERR(file_w)) 
	{
		goto WriteFileOpenErr;
	}
	else 
	{
		offset = (PCHAR) rtstrstr((PSTRING) cfgData, "Default\n");
		offset += strlen("Default\n");
		RtmpOSFileWrite(file_w, (PSTRING)cfgData, (int)(offset-cfgData));
		os_alloc_mem(NULL, (UCHAR **)&pTempStr, 512);
		if (!pTempStr)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("pTempStr kmalloc fail. (512)\n"));
			RtmpOSFileClose(file_w);
			goto WriteErr;
		}
			
		for (;;)
		{
			int i = 0;
			PSTRING ptr;

			NdisZeroMemory(pTempStr, 512);
			ptr = (PSTRING) offset;
			while(*ptr && *ptr != '\n')
			{
				pTempStr[i++] = *ptr++;
			}
			pTempStr[i] = 0x00;
			if ((size_t)(offset - cfgData) < fileLen)
			{
				offset += strlen(pTempStr) + 1;
				if (strncmp(pTempStr, "SSID=", strlen("SSID=")) == 0)
				{
					NdisZeroMemory(pTempStr, 512);
					NdisMoveMemory(pTempStr, "SSID=", strlen("SSID="));
					NdisMoveMemory(pTempStr + 5, pAd->CommonCfg.Ssid, pAd->CommonCfg.SsidLen);
				}
				else if (strncmp(pTempStr, "AuthMode=", strlen("AuthMode=")) == 0)
				{
					NdisZeroMemory(pTempStr, 512);
					if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeOpen)
						sprintf(pTempStr, "AuthMode=OPEN");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeShared)
						sprintf(pTempStr, "AuthMode=SHARED");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeAutoSwitch)
						sprintf(pTempStr, "AuthMode=WEPAUTO");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPAPSK)
						sprintf(pTempStr, "AuthMode=WPAPSK");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA2PSK)
						sprintf(pTempStr, "AuthMode=WPA2PSK");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA)
						sprintf(pTempStr, "AuthMode=WPA");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA2)
						sprintf(pTempStr, "AuthMode=WPA2");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPANone)
						sprintf(pTempStr, "AuthMode=WPANONE");
				}
				else if (strncmp(pTempStr, "EncrypType=", strlen("EncrypType=")) == 0)
				{
					NdisZeroMemory(pTempStr, 512);
					if (pAd->StaCfg.WepStatus == Ndis802_11WEPDisabled)
						sprintf(pTempStr, "EncrypType=NONE");
					else if (pAd->StaCfg.WepStatus == Ndis802_11WEPEnabled)
						sprintf(pTempStr, "EncrypType=WEP");
					else if (pAd->StaCfg.WepStatus == Ndis802_11Encryption2Enabled)
						sprintf(pTempStr, "EncrypType=TKIP");
					else if (pAd->StaCfg.WepStatus == Ndis802_11Encryption3Enabled)
						sprintf(pTempStr, "EncrypType=AES");
				}
				RtmpOSFileWrite(file_w, pTempStr, strlen(pTempStr));
				RtmpOSFileWrite(file_w, "\n", 1);
			}
			else
			{
				break;
			}
		}
		RtmpOSFileClose(file_w);
	}

WriteErr:   
	if (pTempStr)
/*		kfree(pTempStr); */
		os_free_mem(NULL, pTempStr);
ReadErr:
WriteFileOpenErr:    
	if (cfgData)
/*		kfree(cfgData); */
		os_free_mem(NULL, cfgData);
out:
	RtmpOSFSInfoChange(&osFSInfo, FALSE);


	DBGPRINT(RT_DEBUG_TRACE, ("<----- WriteConfToDatFile\n"));
	return;
}


INT write_dat_file_thread (
    IN ULONG Context)
{
	RTMP_OS_TASK *pTask;
	RTMP_ADAPTER *pAd;
	//int 	Status = 0;

	pTask = (RTMP_OS_TASK *)Context;

	if (pTask == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("%s: pTask is NULL\n", __FUNCTION__));
		return 0;
	}
	
	pAd = (PRTMP_ADAPTER)RTMP_OS_TASK_DATA_GET(pTask);

	if (pAd == NULL)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd is NULL\n", __FUNCTION__));
		return 0;
	}

	RtmpOSTaskCustomize(pTask);

	/* Update ssid, auth mode and encr type to DAT file */
	WriteConfToDatFile(pAd);
	
		RtmpOSTaskNotifyToExit(pTask);
	
	return 0;
}

NDIS_STATUS WriteDatThread(
	IN  RTMP_ADAPTER *pAd)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	RTMP_OS_TASK *pTask;

	if (pAd->bWriteDat == FALSE)
		return 0;

	DBGPRINT(RT_DEBUG_TRACE, ("-->WriteDatThreadInit()\n"));

	pTask = &pAd->WriteDatTask;

	RTMP_OS_TASK_INIT(pTask, "RtmpWriteDatTask", pAd);
	status = RtmpOSTaskAttach(pTask, write_dat_file_thread, (ULONG)&pAd->WriteDatTask);
	DBGPRINT(RT_DEBUG_TRACE, ("<--WriteDatThreadInit(), status=%d!\n", status));

	return status;
}
#endif /* PROFILE_STORE */
#endif /* CONFIG_STA_SUPPORT */

/* End of rtmp_init_inf.c */
