/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2013 by Amaury Pouly
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
#include "regs-pinctrl.h"
#include "regs-power.h"
#include "regs-lradc.h"
#include "regs-digctl.h"

#define BOOT_ROM_CONTINUE   0 /* continue boot */
#define BOOT_ROM_SECTION    1 /* switch to new section *result_id */

typedef unsigned long uint32_t;

// target specific boot context
enum context_t
{
    CONTEXT_NORMAL, /* normal boot */
    CONTEXT_USB, /* USB plugged boot */
    CONTEXT_RTC, /* RTC wake up boot */
};
// target specific boot decision
enum boot_t
{
    BOOT_STOP,  /* power down */
    BOOT_ROCK, /* boot to Rockbox */
    BOOT_OF, /* boot to OF */
};

/**
 * Helper functions
 */

static inline int __attribute__((always_inline)) read_gpio(int bank, int pin)
{
    return (HW_PINCTRL_DINn(bank) >> pin) & 1;
}

static inline int __attribute__((always_inline)) read_pswitch(void)
{
#if IMX233_SUBTARGET >= 3700
    return BF_RD(POWER_STS, PSWITCH);
#else
    return BF_RD(DIGCTL_STATUS, PSWITCH);
#endif
}

/* only works for channels <=7, always divide by 2, never accumulates */
static inline void __attribute__((always_inline)) setup_lradc(int src)
{
    BF_CLR(LRADC_CTRL0, SFTRST);
    BF_CLR(LRADC_CTRL0, CLKGATE);
    /* don't bother changing the source, we are early enough at boot so that
     * channel x is mapped to source x */
    HW_LRADC_CHn_CLR(src) = BM_OR2(LRADC_CHn, NUM_SAMPLES, ACCUMULATE);
    BF_SETV(LRADC_CTRL2, DIVIDE_BY_TWO, 1 << src);
}

#define BP_LRADC_CTRL1_LRADCx_IRQ(x)    (x)
#define BM_LRADC_CTRL1_LRADCx_IRQ(x)    (1 << (x))

static inline int __attribute__((always_inline)) read_lradc(int src)
{
    BF_CLR(LRADC_CTRL1, LRADCx_IRQ(src));
    BF_SETV(LRADC_CTRL0, SCHEDULE, 1 << src);
    while(!BF_RD(LRADC_CTRL1, LRADCx_IRQ(src)));
    return BF_RDn(LRADC_CHn, src, VALUE);
}

static inline void __attribute__((noreturn)) power_down()
{
#ifdef SANSA_FUZEPLUS
    /* B0P09: this pin seems to be important to shutdown the hardware properly */
    HW_PINCTRL_MUXSELn_SET(0) = 3 << 18;
    HW_PINCTRL_DOEn(0) = 1 << 9;
    HW_PINCTRL_DOUTn(0) = 1 << 9;
#endif
    /* power down */
    HW_POWER_RESET = BM_OR2(POWER_RESET, UNLOCK, PWD);
    while(1);
}

/**
 * Boot decision functions
 */

#if defined(SANSA_FUZEPLUS)
static enum boot_t boot_decision(enum context_t context)
{
    /* if volume down is hold, boot to OF */
    if(!read_gpio(1, 30))
        return BOOT_OF;
    /* on normal boot, make sure power button is hold long enough */
    if(context == CONTEXT_NORMAL)
    {
        // monitor PSWITCH
        int count = 0;
        for(int i = 0; i < 550000; i++)
            if(read_pswitch() == 1)
                count++;
        if(count < 400000)
            return BOOT_STOP;
    }
    return BOOT_ROCK;
}
#elif defined(CREATIVE_ZENXFI2)
static int boot_decision(int context)
{
    /* We are lacking buttons on the Zen X-Fi2 because on USB, the select button
     * enters recovery mode ! So we can only use power but power is used to power up
     * on normal boots and then select is free ! Thus use a non-uniform scheme:
     * - normal boot/RTC:
     *   - no key: Rockbox
     *   - select: OF
     * - USB boot:
     *   - no key: Rockbox
     *   - power: OF
     */
    if(context == CONTEXT_USB)
        return read_pswitch() == 1 ? BOOT_OF : BOOT_ROCK;
    else
        return !read_gpio(0, 14) ? BOOT_OF : BOOT_ROCK;
}
#elif defined(CREATIVE_ZENXFI3)
static int boot_decision(int context)
{
    /* if volume down is hold, boot to OF */
    return !read_gpio(2, 7) ? BOOT_OF : BOOT_ROCK;
}
#elif defined(SONY_NWZE360) || defined(SONY_NWZE370)
static int local_decision(void)
{
    /* read keys and pswitch */
    int val = read_lradc(0);
    /* if hold is on, power off
     * if back is pressed, boot to OF
     * if play is pressed, boot RB
     * otherwise power off */
    if(read_gpio(0, 9) == 0)
        return BOOT_STOP;
    if(val >= 1050 && val < 1150)
        return BOOT_OF;
    if(val >= 1420 && val < 1520)
        return BOOT_ROCK;
    return BOOT_STOP;
}

static int boot_decision(int context)
{
    setup_lradc(0); // setup LRADC channel 0 to read keys
    HW_PINCTRL_PULLn_SET(0) = 1 << 9; // enable pullup on hold key (B0P09)
    /* make a decision */
    int decision = local_decision();
    /* in USB or alarm context, stick to it */
    if(context == CONTEXT_USB || context == CONTEXT_RTC)
    {
        /* never power down so replace power off decision by rockbox */
        return decision == BOOT_STOP ? BOOT_ROCK : decision;
    }
    /* otherwise start a 1 second timeout. Any decision change
     * will result in power down */
    uint32_t tmo = HW_DIGCTL_MICROSECONDS + 1000000;
    while(HW_DIGCTL_MICROSECONDS < tmo)
    {
        int new_dec = local_decision();
        if(new_dec != decision)
            return BOOT_STOP;
    }
    return decision;
}
#else
#warning You should define a target specific boot decision function
static int boot_decision(int context)
{
    return BOOT_ROCK;
}
#endif

/**
 * Context functions
 */
static inline enum context_t get_context(void)
{
#if IMX233_SUBTARGET >= 3780
    /* On the imx233 it's easy because we know the power up source */
    unsigned pwrup_src = BF_RD(POWER_STS, PWRUP_SOURCE);
    if(pwrup_src & (1 << 5))
        return CONTEXT_USB;
    else if(pwrup_src & (1 << 4))
        return CONTEXT_RTC;
    else
        return CONTEXT_NORMAL;
#else
    /* On the other targets, we need to poke a few more registers */
#endif
}

int main(uint32_t arg, uint32_t *result_id)
{
    switch(boot_decision(get_context()))
    {
        case BOOT_ROCK:
            *result_id = arg;
            return BOOT_ROM_SECTION;
        case BOOT_OF:
            return BOOT_ROM_CONTINUE;
        case BOOT_STOP:
        default:
            power_down();
    }
}

int __attribute__((section(".start"))) start(uint32_t arg, uint32_t *result_id)
{
    return main(arg, result_id);
}