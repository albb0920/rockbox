/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2011 by Amaury Pouly
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"
#include "system.h"
#include "dma-imx233.h"
#include "lcd.h"
#include "string.h"

// statistics about unaligned transfers
static int apb_nr_unaligned[32];

void imx233_dma_init(void)
{
    /* Enable APHB and APBX */
    imx233_reset_block(&HW_APBH_CTRL0);
    imx233_reset_block(&HW_APBX_CTRL0);
}

void imx233_dma_reset_channel(unsigned chan)
{
    uint32_t bm = 1 << APB_GET_DMA_CHANNEL(chan);
    if(APB_IS_APBX_CHANNEL(chan))
    {
#if IMX233_SUBTARGET < 3780
        BF_SETV(APBX_CTRL0, RESET_CHANNEL, bm);
        while(BF_RD(APBX_CTRL0, RESET_CHANNEL) & bm);
#else
        BF_SETV(APBX_CHANNEL_CTRL, RESET_CHANNEL, bm);
        while(BF_RD(APBX_CHANNEL_CTRL, RESET_CHANNEL) & bm);
#endif
    }
    else
    {
        BF_SETV(APBH_CTRL0, RESET_CHANNEL, bm);
        while(BF_RD(APBH_CTRL0, RESET_CHANNEL) & bm);
    }
}

void imx233_dma_clkgate_channel(unsigned chan, bool enable_clock)
{
    if(APB_IS_APBX_CHANNEL(chan))
        return;
    if(enable_clock)
        BF_CLRV(APBH_CTRL0, CLKGATE_CHANNEL, 1 << APB_GET_DMA_CHANNEL(chan));
    else
        BF_SETV(APBH_CTRL0, CLKGATE_CHANNEL, 1 << APB_GET_DMA_CHANNEL(chan));
}

void imx233_dma_freeze_channel(unsigned chan, bool freeze)
{
    uint32_t bm = 1 << APB_GET_DMA_CHANNEL(chan);
    if(APB_IS_APBX_CHANNEL(chan))
    {
#if IMX233_SUBTARGET < 3780
        if(freeze)
            BF_SETV(APBX_CTRL0, FREEZE_CHANNEL, bm);
        else
            BF_CLRV(APBX_CTRL0, FREEZE_CHANNEL, bm);
#else
        if(freeze)
            BF_SETV(APBX_CHANNEL_CTRL, FREEZE_CHANNEL, bm);
        else
            BF_CLRV(APBX_CHANNEL_CTRL, FREEZE_CHANNEL, bm);
#endif
    }
    else
    {
        if(freeze)
            BF_SETV(APBH_CTRL0, FREEZE_CHANNEL, bm);
        else
            BF_CLRV(APBH_CTRL0, FREEZE_CHANNEL, bm);
    }
}

void imx233_dma_enable_channel_interrupt(unsigned chan, bool enable)
{
    uint32_t bm = 1 << APB_GET_DMA_CHANNEL(chan);
    if(APB_IS_APBX_CHANNEL(chan))
    {
        if(enable)
            BF_SETV(APBX_CTRL1, CH_CMDCMPLT_IRQ_EN, bm);
        else
            BF_CLRV(APBX_CTRL1, CH_CMDCMPLT_IRQ_EN, bm);
    }
    else
    {
        if(enable)
            BF_SETV(APBH_CTRL1, CH_CMDCMPLT_IRQ_EN, bm);
        else
            BF_CLRV(APBH_CTRL1, CH_CMDCMPLT_IRQ_EN, bm);
    }
    imx233_dma_clear_channel_interrupt(chan);
}

void imx233_dma_clear_channel_interrupt(unsigned chan)
{
    uint32_t bm = 1 << APB_GET_DMA_CHANNEL(chan);
    if(APB_IS_APBX_CHANNEL(chan))
    {
        BF_CLRV(APBX_CTRL1, CH_CMDCMPLT_IRQ, bm);
#if IMX233_SUBTARGET >= 3780
        BF_CLRV(APBX_CTRL2, CH_ERROR_IRQ, bm);
#elif IMX233_SUBTARGET >= 3700
        BF_CLRV(APBX_CTRL1, CH_AHB_ERROR_IRQ, bm);
#endif
    }
    else
    {
        BF_CLRV(APBH_CTRL1, CH_CMDCMPLT_IRQ, bm);
#if IMX233_SUBTARGET >= 3780
        BF_CLRV(APBH_CTRL2, CH_ERROR_IRQ, bm);
#elif IMX233_SUBTARGET >= 3700
        BF_CLRV(APBH_CTRL1, CH_AHB_ERROR_IRQ, bm);
#endif
    }
}

bool imx233_dma_is_channel_error_irq(unsigned chan)
{
    uint32_t bm = 1 << APB_GET_DMA_CHANNEL(chan);
#if IMX233_SUBTARGET >= 3780
    if(APB_IS_APBX_CHANNEL(chan))
        return !!(BF_RD(APBX_CTRL2, CH_ERROR_IRQ) & bm);
    else
        return !!(BF_RD(APBH_CTRL2, CH_ERROR_IRQ) & bm);
#elif IMX233_SUBTARGET >= 3700
    if(APB_IS_APBX_CHANNEL(chan))
        return !!(BF_RD(APBX_CTRL1, CH_AHB_ERROR_IRQ) & bm);
    else
        return !!(BF_RD(APBH_CTRL1, CH_AHB_ERROR_IRQ) & bm);
#else
    (void) bm;
    return false;
#endif
}

/* Commit and/or discard all DMA descriptors and buffers pointed by them,
 * handle circular lists. At the same time, convert virtual pointers to
 * real ones */
void imx233_dma_prepare_command(unsigned chan, struct apb_dma_command_t *cmd)
{
    /* We handle circular descriptors by using unused bits:
     * bits 8-11 are not used by the hardware so we first go through the whole
     * list and mark them all a special value at the same time we commit buffers
     * and then we go through the list another time to clear the mark and
     * commit the descriptors */
    struct apb_dma_command_t *cur = cmd;

    while(BF_RDX(cur->cmd, APB_CHx_CMD, UNUSED) != BV_APB_CHx_CMD_UNUSED__MAGIC)
    {
        BF_WR_VX(cur->cmd, APB_CHx_CMD, UNUSED, MAGIC);
        int op = BF_RDX(cur->cmd, APB_CHx_CMD, COMMAND);
        int sz = BF_RDX(cur->cmd, APB_CHx_CMD, XFER_COUNT);
        /* device > host: discard */
        if(op == BV_APB_CHx_CMD_COMMAND__WRITE)
            discard_dcache_range(cur->buffer, sz);
        /* host > device: commit and discard */
        else if(op == BV_APB_CHx_CMD_COMMAND__READ)
            commit_discard_dcache_range(cur->buffer, sz);
        if((uint32_t)cur->buffer % CACHEALIGN_SIZE)
            apb_nr_unaligned[chan]++;
        /* Virtual to physical buffer pointer conversion */
        cur->buffer = PHYSICAL_ADDR(cur->buffer);
        /* chain ? */
        if(cur->cmd & BM_APB_CHx_CMD_CHAIN)
            cur = cur->next;
        else
            break;
    }

    cur = cmd;
    while(BF_RDX(cur->cmd, APB_CHx, CMD_UNUSED) != 0)
    {
        BF_WRX(cur->cmd, APB_CHx, CMD_UNUSED, 0);
        int sz = BF_RDX(cur->cmd, APB_CHx_CMD, CMDWORDS) * sizeof(uint32_t);
        /* commit descriptor and discard descriptor */
        /* chain ? */
        if(cur->cmd & BM_APB_CHx_CMD_CHAIN)
        {
            struct apb_dma_command_t *next = cur->next;
            cur->next = PHYSICAL_ADDR(cur->next);
            commit_dcache_range(cur, sizeof(struct apb_dma_command_t) + sz);
            cur = next;
        }
        else
        {
            commit_dcache_range(cur, sizeof(struct apb_dma_command_t) + sz);
            break;
        }
    }
}

void imx233_dma_set_next_command(unsigned chan, struct apb_dma_command_t *cmd)
{
    if(APB_IS_APBX_CHANNEL(chan))
        HW_APBX_CHn_NXTCMDAR(APB_GET_DMA_CHANNEL(chan)) = (uint32_t)PHYSICAL_ADDR(cmd);
    else
        HW_APBH_CHn_NXTCMDAR(APB_GET_DMA_CHANNEL(chan)) = (uint32_t)PHYSICAL_ADDR(cmd);
}

void imx233_dma_inc_sema(unsigned chan, unsigned amount)
{
    if(APB_IS_APBX_CHANNEL(chan))
        HW_APBX_CHn_SEMA(APB_GET_DMA_CHANNEL(chan)) = amount;
    else
        HW_APBH_CHn_SEMA(APB_GET_DMA_CHANNEL(chan)) = amount;
}

void imx233_dma_start_command(unsigned chan, struct apb_dma_command_t *cmd)
{
    imx233_dma_prepare_command(chan, cmd);
    imx233_dma_set_next_command(chan, cmd);
    imx233_dma_inc_sema(chan, 1);
}

int imx233_dma_wait_completion(unsigned chan, unsigned tmo)
{
    tmo += current_tick;
    int value = 0;
    if(APB_IS_APBX_CHANNEL(chan))
        while((value = BF_RDn(APBX_CHn_SEMA, APB_GET_DMA_CHANNEL(chan), PHORE)) && !TIME_AFTER(current_tick, tmo))
            yield();
    else
        while((value = BF_RDn(APBH_CHn_SEMA, APB_GET_DMA_CHANNEL(chan), PHORE)) && !TIME_AFTER(current_tick, tmo))
            yield();

    return value;
}

struct imx233_dma_info_t imx233_dma_get_info(unsigned chan, unsigned flags)
{
    struct imx233_dma_info_t s;
    memset(&s, 0, sizeof(s));
    bool apbx = APB_IS_APBX_CHANNEL(chan);
    int dmac = APB_GET_DMA_CHANNEL(chan);
    uint32_t bm = 1 << dmac;
    if(flags & DMA_INFO_CURCMDADDR)
        s.cur_cmd_addr = apbx ? HW_APBX_CHn_CURCMDAR(dmac) : HW_APBH_CHn_CURCMDAR(dmac);
    if(flags & DMA_INFO_NXTCMDADDR)
        s.nxt_cmd_addr = apbx ? HW_APBX_CHn_NXTCMDAR(dmac) : HW_APBH_CHn_NXTCMDAR(dmac);
    if(flags & DMA_INFO_CMD)
        s.cmd = apbx ? HW_APBX_CHn_CMD(dmac) : HW_APBH_CHn_CMD(dmac);
    if(flags & DMA_INFO_BAR)
        s.bar = apbx ? HW_APBX_CHn_BAR(dmac) : HW_APBH_CHn_BAR(dmac);
    if(flags & DMA_INFO_AHB_BYTES)
        s.ahb_bytes = apbx ? BF_RDn(APBX_CHn_DEBUG2, dmac, AHB_BYTES) : BF_RDn(APBH_CHn_DEBUG2, dmac, AHB_BYTES);
    if(flags & DMA_INFO_APB_BYTES)
        s.apb_bytes = apbx ? BF_RDn(APBX_CHn_DEBUG2, dmac, APB_BYTES) : BF_RDn(APBH_CHn_DEBUG2, dmac, APB_BYTES);
    if(flags & DMA_INFO_FREEZED)
#if IMX233_SUBTARGET < 3780
        s.freezed = !!((apbx ? BF_RD(APBX_CTRL0, FREEZE_CHANNEL) : BF_RD(APBH_CTRL0, FREEZE_CHANNEL)) & bm);
#else
        s.freezed = !!((apbx ? BF_RD(APBX_CHANNEL_CTRL, FREEZE_CHANNEL) : BF_RD(APBH_CTRL0, FREEZE_CHANNEL)) & bm);
#endif
    if(flags & DMA_INFO_GATED)
        s.gated = apbx ? false : !!(BF_RD(APBH_CTRL0, CLKGATE_CHANNEL) & bm);
    if(flags & DMA_INFO_INTERRUPT)
    {
        s.int_enabled = !!((apbx ? BF_RD(APBX_CTRL1, CH_CMDCMPLT_IRQ_EN) : BF_RD(APBH_CTRL1, CH_CMDCMPLT_IRQ_EN)) & bm);
        s.int_cmdcomplt = !!((apbx ? BF_RD(APBX_CTRL1, CH_CMDCMPLT_IRQ) : BF_RD(APBH_CTRL1, CH_CMDCMPLT_IRQ)) & bm);
#if IMX233_SUBTARGET >= 3780
        s.int_error = !!((apbx ? BF_RD(APBX_CTRL2, CH_ERROR_IRQ) : BF_RD(APBH_CTRL2, CH_ERROR_IRQ)) & bm);
#elif IMX233_SUBTARGET >= 3700
        s.int_error = !!((apbx ? BF_RD(APBX_CTRL1, CH_AHB_ERROR_IRQ) : BF_RD(APBH_CTRL1, CH_AHB_ERROR_IRQ)) & bm);
#else
        s.int_error = false;
#endif
    }
    s.nr_unaligned = apb_nr_unaligned[chan];
    return s;
}

