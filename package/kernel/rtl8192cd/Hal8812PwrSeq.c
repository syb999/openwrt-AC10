
/*++
Copyright (c) Realtek Semiconductor Corp. All rights reserved.

Module Name:
	Hal8812PwrSeq.c

Abstract:
	This file includes all kinds of Power Action event for RTL8812 and corresponding hardware configurtions which are released from HW SD.

Major Change History:
	When       Who               What
	---------- ---------------   -------------------------------
	2011-10-26 William            Create.

--*/


#ifdef CONFIG_RTL_8812_SUPPORT

#include "HalPwrSeqCmd.h"
#include "Hal8812PwrSeq.h"

/*
    drivers should parse below arrays and do the corresponding actions
*/
//3 Power on  Array
WLAN_PWR_CFG rtl8812_power_on_flow[RTL8812_TRANS_CARDEMU_TO_ACT_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_CARDEMU_TO_ACT
	RTL8812_TRANS_END
};

//3Radio off GPIO Array
WLAN_PWR_CFG rtl8812_radio_off_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_ACT_TO_CARDEMU
	RTL8812_TRANS_END
};

//3Card Disable Array
WLAN_PWR_CFG rtl8812_card_disable_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_CARDEMU_TO_PDN_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_ACT_TO_CARDEMU
	RTL8812_TRANS_CARDEMU_TO_CARDDIS
	RTL8812_TRANS_END
};

//3 Card Enable Array
WLAN_PWR_CFG rtl8812_card_enable_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_CARDEMU_TO_PDN_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_CARDDIS_TO_CARDEMU
	RTL8812_TRANS_CARDEMU_TO_ACT
	RTL8812_TRANS_END
};

//3Suspend Array
WLAN_PWR_CFG rtl8812_suspend_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_CARDEMU_TO_SUS_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_ACT_TO_CARDEMU
	RTL8812_TRANS_CARDEMU_TO_SUS
	RTL8812_TRANS_END
};

//3 Resume Array
WLAN_PWR_CFG rtl8812_resume_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_CARDEMU_TO_SUS_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_SUS_TO_CARDEMU
	RTL8812_TRANS_CARDEMU_TO_ACT
	RTL8812_TRANS_END
};



//3HWPDN Array
WLAN_PWR_CFG rtl8812_hwpdn_flow[RTL8812_TRANS_ACT_TO_CARDEMU_STEPS+RTL8812_TRANS_CARDEMU_TO_PDN_STEPS+RTL8812_TRANS_END_STEPS]=
{
	RTL8812_TRANS_ACT_TO_CARDEMU
	RTL8812_TRANS_CARDEMU_TO_PDN
	RTL8812_TRANS_END
};

//3 Enter LPS
WLAN_PWR_CFG rtl8812_enter_lps_flow[RTL8812_TRANS_ACT_TO_LPS_STEPS+RTL8812_TRANS_END_STEPS]=
{
	//FW behavior
	RTL8812_TRANS_ACT_TO_LPS
	RTL8812_TRANS_END
};

//3 Leave LPS
WLAN_PWR_CFG rtl8812_leave_lps_flow[RTL8812_TRANS_LPS_TO_ACT_STEPS+RTL8812_TRANS_END_STEPS]=
{
	//FW behavior
	RTL8812_TRANS_LPS_TO_ACT
	RTL8812_TRANS_END
};

#endif //CONFIG_RTL_8812_SUPPORT

