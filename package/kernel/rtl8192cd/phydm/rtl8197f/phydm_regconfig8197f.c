/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#include "mp_precomp.h"
#include "../phydm_precomp.h"

#if (RTL8197F_SUPPORT == 1)

void
odm_ConfigRFReg_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Data,
	IN	ODM_RF_RADIO_PATH_E		RF_PATH,
	IN	u4Byte					RegAddr
	)
{
	if ((Addr == 0xfe) || (Addr == 0xffe)) {
		#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_ms(50);
		#else
		ODM_delay_ms(50);
		#endif
	} else {
		ODM_SetRFReg(pDM_Odm, RF_PATH, RegAddr, bRFRegOffsetMask, Data);

		/* Add 1us delay between BB/RF register setting. */
		ODM_delay_us(1);
	}
}

void
odm_ConfigRF_RadioA_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Data
	)
{
	u4Byte	content = 0x1000;							/* RF_Content: radioa_txt */
	u4Byte	maskforPhySet = (u4Byte)(content&0xE000);

	odm_ConfigRFReg_8197F(pDM_Odm, Addr, Data, ODM_RF_PATH_A, Addr|maskforPhySet);

	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_TRACE, ("===> ODM_ConfigRFWithHeaderFile: [RadioA] %08X %08X\n", Addr, Data));
}

void
odm_ConfigRF_RadioB_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Data
	)
{
	u4Byte	content = 0x1001;							/* RF_Content: radiob_txt */
	u4Byte	maskforPhySet = (u4Byte)(content&0xE000);

	odm_ConfigRFReg_8197F(pDM_Odm, Addr, Data, ODM_RF_PATH_B, Addr|maskforPhySet);

	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_TRACE, ("===> ODM_ConfigRFWithHeaderFile: [RadioB] %08X %08X\n", Addr, Data));
}

void
odm_ConfigMAC_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u1Byte					Data
	)
{
	ODM_Write1Byte(pDM_Odm, Addr, Data);
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_TRACE, ("===> ODM_ConfigMACWithHeaderFile: [MAC_REG] %08X %08X\n", Addr, Data));
}

void
odm_UpdateAgcBigJumpLmt_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Data
)
{
	pDIG_T	pDM_DigTable = &pDM_Odm->DM_DigTable;
	u1Byte	rfGainIdx = (u1Byte)((Data & 0x0FF00000) >> 20);
	u1Byte	bbGainIdx = (u1Byte)(((Data & 0x0000ff00) >> 8)) << 1;	/*97F gain idx need multiply 2*/
	u1Byte	agcTableIdx = (u1Byte)((Data & 0x0000f000) >> 12);
	u1Byte	bigJumpLmtIdx = 0;
	static BOOLEAN isLimit;

	if (agcTableIdx > 3)
		bigJumpLmtIdx = 1;
	else
		bigJumpLmtIdx = 0;

	if (Addr != 0xc78)
		return;

	if (bbGainIdx > 0x3c) {
		if ((rfGainIdx == pDM_DigTable->rfGainIdx) && (isLimit == FALSE)) {
			isLimit = TRUE;
			pDM_DigTable->bigJumpLmt[bigJumpLmtIdx] = bbGainIdx - 2;
			ODM_RT_TRACE(pDM_Odm, ODM_COMP_DIG, ODM_DBG_TRACE, ("===> [AGC_TAB] bigJumpLmt [%d] = 0x%x\n", bigJumpLmtIdx, pDM_DigTable->bigJumpLmt[bigJumpLmtIdx]));
		}
	} else
		isLimit = FALSE;

	pDM_DigTable->rfGainIdx = rfGainIdx;

}

void
odm_ConfigBB_AGC_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Bitmask,
	IN	u4Byte					Data
	)
{
	odm_UpdateAgcBigJumpLmt_8197F(pDM_Odm, Addr, Data);

	ODM_SetBBReg(pDM_Odm, Addr, Bitmask, Data);

	/* Add 1us delay between BB/RF register setting. */
	ODM_delay_us(1);

	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_TRACE, ("===> ODM_ConfigBBWithHeaderFile: [AGC_TAB] %08X %08X\n", Addr, Data));
}

void
odm_ConfigBB_PHY_REG_PG_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Band,
	IN	u4Byte					RfPath,
	IN	u4Byte					TxNum,
	IN	u4Byte					Addr,
	IN	u4Byte					Bitmask,
	IN	u4Byte					Data
	)
{
	if (Addr == 0xfe || Addr == 0xffe) {
		#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_ms(50);
		#else
		ODM_delay_ms(50);
		#endif
	} else {
#if (!(DM_ODM_SUPPORT_TYPE&ODM_AP))
	    PHY_StoreTxPowerByRate(pDM_Odm->Adapter, Band, RfPath, TxNum, Addr, Bitmask, Data);
#endif

	}
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_LOUD, ("===> ODM_ConfigBBWithHeaderFile: [PHY_REG] %08X %08X %08X\n", Addr, Bitmask, Data));
}

void
odm_ConfigBB_PHY_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	u4Byte					Addr,
	IN	u4Byte					Bitmask,
	IN	u4Byte					Data
	)
{
	if (Addr == 0xfe)
		#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_ms(50);
		#else
		ODM_delay_ms(50);
		#endif
	else if (Addr == 0xfd)
		ODM_delay_ms(5);
	else if (Addr == 0xfc)
		ODM_delay_ms(1);
	else if (Addr == 0xfb)
		ODM_delay_us(50);
	else if (Addr == 0xfa)
		ODM_delay_us(5);
	else if (Addr == 0xf9)
		ODM_delay_us(1);
	else
		ODM_SetBBReg(pDM_Odm, Addr, Bitmask, Data);

	/* Add 1us delay between BB/RF register setting. */
	ODM_delay_us(1);
	ODM_RT_TRACE(pDM_Odm, ODM_COMP_INIT, ODM_DBG_TRACE, ("===> ODM_ConfigBBWithHeaderFile: [PHY_REG] %08X %08X\n", Addr, Data));
}

void
odm_ConfigBB_TXPWR_LMT_8197F(
	IN	PDM_ODM_T				pDM_Odm,
	IN	pu1Byte					Regulation,
	IN	pu1Byte					Band,
	IN	pu1Byte					Bandwidth,
	IN	pu1Byte					RateSection,
	IN	pu1Byte					RfPath,
	IN	pu1Byte					Channel,
	IN	pu1Byte					PowerLimit
	)
{
#if (DM_ODM_SUPPORT_TYPE & (ODM_WIN|ODM_CE))
	PHY_SetTxPowerLimit(pDM_Odm, Regulation, Band,
		Bandwidth, RateSection, RfPath, Channel, PowerLimit);
#endif
}

#endif

