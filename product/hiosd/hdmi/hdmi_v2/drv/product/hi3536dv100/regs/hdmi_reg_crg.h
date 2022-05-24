//******************************************************************************
// Copyright     :  Copyright (C) 2016, Hisilicon Technologies Co., Ltd.
// File name     :  hi_reg_crg.h
// Author        :  xxx
// Version       :  1.0
// Date          :  2016-01-20
// Description   :  Define all registers/tables for S40V200
// Others        :  Generated automatically by nManager V4.0
// History       :  xxx 2016-01-20 Create file
//******************************************************************************

#ifndef __HDMI_REG_CRG_H__
#define __HDMI_REG_CRG_H__

/* Define the union R_PERI_CRG59 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ssc_in_cken                 : 1   ; /* [0]  */
        unsigned int    ssc_bypass_cken             : 1   ; /* [1]  */
        unsigned int    hdmitx_ctrl_osc_24m_cken    : 1   ; /* [2]  */
        unsigned int    hdmitx_ctrl_cec_cken        : 1   ; /* [3]  */
        unsigned int    hdmitx_ctrl_os_cken         : 1   ; /* [4]  */
        unsigned int    hdmitx_ctrl_as_cken         : 1   ; /* [5]  */
        unsigned int    hdmitx_ctrl_bus_srst_req    : 1   ; /* [6]  */
        unsigned int    hdmitx_ctrl_srst_req        : 1   ; /* [7]  */
        unsigned int    hdmitx_ctrl_cec_srst_req    : 1   ; /* [8]  */
        unsigned int    hdmitx_ssc_srst_req         : 1   ; /* [9]  */
        unsigned int    ssc_clk_div                 : 4   ; /* [13..10]  */
        unsigned int    reserved_0                  : 2   ; /* [15..14]  */
        unsigned int    hdmitx_pxl_cken             : 1   ; /* [16]  */
        unsigned int    reserved_1                  : 2   ; /* [18..17]  */
        unsigned int    ssc_bypass_clk_sel          : 1   ; /* [19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG35;

/* Define the union R_PERI_CRG60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    phy_tmds_cken         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    hdmitx_phy_srst_req   : 1   ; /* [4]  */
        unsigned int    phy_tmds_srst_req     : 1   ; /* [5]  */
        unsigned int    tmds_clk_div          : 3   ; /* [8..6]  */
        unsigned int    reserved_1            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} R_PERI_CRG36;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int           reserved_0[35]                     ; /* 0x00~0x88  */
    volatile R_PERI_CRG35           PERI_CRG35            ; /* 0x8c */
    volatile R_PERI_CRG36           PERI_CRG36            ; /* 0x90 */
    volatile unsigned int           reserved_1[28]                     ; /* 0x94~0x104 */
} HDMI_REG_CRG_S;

//extern volatile HDMI_REG_CRG_S *s_pstRegCrg;

int REG_HDMI_CRG_reg_Init(void);
int REG_HDMI_CRG_reg_DeInit(void);


//hdmitx ctrl
int REG_HDMI_CRG_ssc_in_cken_Set(unsigned int ussc_in_cken);
int REG_HDMI_CRG_ssc_bypass_cken_Set(unsigned int ussc_bypass_cken);
int REG_HDMI_CRG_hdmitx_ctrl_osc_24m_cken_Set(unsigned int uhdmitx_ctrl_osc_24m_cken);
int REG_HDMI_CRG_hdmitx_ctrl_cec_cken_Set(unsigned int uhdmitx_ctrl_cec_cken);
int REG_HDMI_CRG_hdmitx_ctrl_os_cken_Set(unsigned int uhdmitx_ctrl_os_cken);
int REG_HDMI_CRG_hdmitx_ctrl_as_cken_Set(unsigned int uhdmitx_ctrl_as_cken);
int REG_HDMI_CRG_hdmitx_ctrl_bus_srst_req_Set(unsigned int uhdmitx_ctrl_bus_srst_req);
int REG_HDMI_CRG_hdmitx_ctrl_srst_req_Set(unsigned int uhdmitx_ctrl_srst_req);
int REG_HDMI_CRG_hdmitx_ctrl_cec_srst_req_Set(unsigned int uhdmitx_ctrl_cec_srst_req);
int REG_HDMI_CRG_hdmitx_ssc_srst_req_Set(unsigned int uhdmitx_ssc_srst_req);
int REG_HDMI_CRG_ssc_clk_div_Set(unsigned int ussc_clk_div);
int REG_HDMI_CRG_hdmitx_pxl_cken_Set(unsigned int uhdmitx_pxl_cken);


//hdmitx phy
int REG_HDMI_CRG_phy_tmds_cken_Set(unsigned int uphy_tmds_cken);
int REG_HDMI_CRG_hdmitx_phy_srst_req_Set(unsigned int uhdmitx_phy_srst_req);
int REG_HDMI_CRG_hdmitx_phy_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_phy_tmds_srst_req_Set(unsigned int uphy_tmds_srst_req);
int REG_HDMI_CRG_phy_tmds_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_hdmitx_phy_srst_req_Get(HI_VOID);
int REG_HDMI_CRG_tmds_clk_div_Set(unsigned int utmds_clk_div);

#endif /* __HDMI_REG_HI3536DV100_CRG_H__ */
