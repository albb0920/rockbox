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

#include "kernel.h"
#include "system.h"
#include "gcc_extensions.h"
#include "system-target.h"
#include "cpu.h"
#include "clkctrl-imx233.h"
#include "pinctrl-imx233.h"
#include "timrot-imx233.h"
#include "dma-imx233.h"
#include "ssp-imx233.h"
#include "i2c-imx233.h"
#if IMX233_SUBTARGET >= 3700
#include "dcp-imx233.h"
#endif
#include "pwm-imx233.h"
#include "icoll-imx233.h"
#include "lradc-imx233.h"
#include "rtc-imx233.h"
#include "power-imx233.h"
#include "emi-imx233.h"
#include "lcd.h"
#include "backlight-target.h"
#include "button.h"
#include "fmradio_i2c.h"
#include "powermgmt-imx233.h"

void imx233_chip_reset(void)
{
#if IMX233_SUBTARGET >= 3700
    HW_CLKCTRL_RESET = BM_CLKCTRL_RESET_CHIP;
#else
    HW_POWER_RESET = BF_OR2(POWER_RESET, UNLOCK_V(KEY), RST_DIG(1));
#endif
}

void system_reboot(void)
{
    _backlight_off();

    disable_irq();

    /* use watchdog to reset */
    imx233_chip_reset();
    while(1);
}

void system_exception_wait(void)
{
    /* make sure lcd and backlight are on */
    lcd_update();
    _backlight_on();
    _backlight_set_brightness(DEFAULT_BRIGHTNESS_SETTING);
    /* wait until button release (if a button is pressed) */
#ifdef HAVE_BUTTON_DATA
    int data;
    while(button_read_device(&data));
    /* then wait until next button press */
    while(!button_read_device(&data));
#else
    while(button_read_device());
    /* then wait until next button press */
    while(!button_read_device());
#endif
}

int system_memory_guard(int newmode)
{
    (void)newmode;
    return 0;
}

static void set_page_tables(void)
{
    /* map every memory region to itself */
    map_section(0, 0, 0x1000, CACHE_NONE);

    /* map RAM and enable caching for it */
    map_section(DRAM_ORIG, CACHED_DRAM_ADDR, MEMORYSIZE, CACHE_ALL);
    map_section(DRAM_ORIG, BUFFERED_DRAM_ADDR, MEMORYSIZE, BUFFERED);
}

void memory_init(void)
{
    ttb_init();
    set_page_tables();
    enable_mmu();
}

void system_init(void)
{
    /* NOTE: don't use anything here that might require tick task !
     * It is initialized by kernel_init *after* system_init().
     * The main() will naturally set cpu speed to normal after kernel_init()
     * so don't bother if the cpu is running at 24MHz here.
     * Make sure IO clock is running at expected speed */
    imx233_clkctrl_init();
    imx233_clkctrl_enable(CLK_PLL, true);
#if IMX233_SUBTARGET >= 3700
    imx233_clkctrl_set_frac_div(CLK_IO, 18); // clk_io@clk_pll
#endif

    imx233_rtc_init();
    imx233_icoll_init();
    imx233_pinctrl_init();
    imx233_timrot_init();
    imx233_dma_init();
    imx233_ssp_init();
#if IMX233_SUBTARGET >= 3700
    imx233_dcp_init();
#endif
    imx233_pwm_init();
    imx233_lradc_init();
    imx233_power_init();
    imx233_i2c_init();
    imx233_powermgmt_init();

    /* make sure auto-slow is disable now, we don't know at which frequency we
     * are running and auto-slow could violate constraints on {xbus,hbus} */
    imx233_clkctrl_enable_auto_slow(false);
    imx233_clkctrl_set_auto_slow_div(BV_CLKCTRL_HBUS_SLOW_DIV__BY8);

    cpu_frequency = imx233_clkctrl_get_freq(CLK_CPU);

#if !defined(BOOTLOADER) && CONFIG_TUNER != 0
    fmradio_i2c_init();
#endif
}

bool imx233_us_elapsed(uint32_t ref, unsigned us_delay)
{
    uint32_t cur = HW_DIGCTL_MICROSECONDS;
    if(ref + us_delay <= ref)
        return !(cur > ref) && !(cur < (ref + us_delay));
    else
        return (cur < ref) || cur >= (ref + us_delay);
}

void imx233_reset_block(volatile uint32_t *block_reg)
{
    /* deassert reset and clock gate */
    __REG_CLR(*block_reg) = __BLOCK_SFTRST;
    while(*block_reg & __BLOCK_SFTRST);
    __REG_CLR(*block_reg) = __BLOCK_CLKGATE;
    while(*block_reg & __BLOCK_CLKGATE);
    /* soft-reset */
    __REG_SET(*block_reg) = __BLOCK_SFTRST;
    /* make sure block is gated off */
    while(!(*block_reg & __BLOCK_CLKGATE));
    /* bring block out of reset */
    __REG_CLR(*block_reg) = __BLOCK_SFTRST;
    while(*block_reg & __BLOCK_SFTRST);
    /* make sure clock is running */
    __REG_CLR(*block_reg) = __BLOCK_CLKGATE;
    while(*block_reg & __BLOCK_CLKGATE);
}

void udelay(unsigned us)
{
    uint32_t ref = HW_DIGCTL_MICROSECONDS;
    while(!imx233_us_elapsed(ref, us));
}

void imx233_digctl_set_arm_cache_timings(unsigned timings)
{
#if IMX233_SUBTARGET >= 3780
    HW_DIGCTL_ARMCACHE = BF_OR5(DIGCTL_ARMCACHE, ITAG_SS(timings),
        DTAG_SS(timings), CACHE_SS(timings), DRTY_SS(timings), VALID_SS(timings));
#else
    HW_DIGCTL_ARMCACHE = BF_OR3(DIGCTL_ARMCACHE, ITAG_SS(timings),
        DTAG_SS(timings), CACHE_SS(timings));
#endif
}

struct cpufreq_profile_t
{
    /* key */
    long cpu_freq;
    /* parameters */
    int vddd, vddd_bo;
    int hbus_div;
    int cpu_idiv, cpu_fdiv;
    long emi_freq;
    int arm_cache_timings;
};

#if IMX233_SUBTARGET >= 3780
static struct cpufreq_profile_t cpu_profiles[] =
{
    /* clk_p@454.74 MHz, clk_h@130.91 MHz, clk_emi@130.91 MHz, VDDD@1.550 V */
    {IMX233_CPUFREQ_454_MHz, 1550, 1450, 3, 1, 19, IMX233_EMIFREQ_130_MHz, 0},
    /* clk_p@261.82 MHz, clk_h@130.91 MHz, clk_emi@130.91 MHz, VDDD@1.275 V */
    {IMX233_CPUFREQ_261_MHz, 1275, 1175, 2, 1, 33, IMX233_EMIFREQ_130_MHz, 0},
    /* clk_p@64 MHz, clk_h@64 MHz, clk_emi@64 MHz, VDDD@1.050 V */
    {IMX233_CPUFREQ_64_MHz, 1050, 975, 1, 5, 27, IMX233_EMIFREQ_64_MHz, 0},
    /* dummy */
    {0, 0, 0, 0, 0, 0, 0, 0}
};
#endif

#define NR_CPU_PROFILES ((int)(sizeof(cpu_profiles)/sizeof(cpu_profiles[0])))

void imx233_set_cpu_frequency(long frequency)
{
#if IMX233_SUBTARGET >= 3780
    /* don't change the frequency if it is useless (changes are expensive) */
    if(cpu_frequency == frequency)
        return;

    struct cpufreq_profile_t *prof = cpu_profiles;
    while(prof->cpu_freq != 0 && prof->cpu_freq != frequency)
        prof++;
    if(prof->cpu_freq == 0)
        return;
    /* disable auto-slow (enable back afterwards) */
    imx233_clkctrl_enable_auto_slow(false);
    /* set VDDIO to the right value */
    imx233_power_set_regulator(REGULATOR_VDDIO, 3300, 3125);

    /* WARNING watch out the order ! */
    if(frequency > cpu_frequency)
    {
        /* Change VDDD regulator */
        imx233_power_set_regulator(REGULATOR_VDDD, prof->vddd, prof->vddd_bo);
        /* Change ARM cache timings */
        imx233_digctl_set_arm_cache_timings(prof->arm_cache_timings);
        /* Switch CPU to crystal at 24MHz */
        imx233_clkctrl_set_bypass(CLK_CPU, true);
        /* Program CPU divider for PLL */
        imx233_clkctrl_set_frac_div(CLK_CPU, prof->cpu_fdiv);
        imx233_clkctrl_set_div(CLK_CPU, prof->cpu_idiv);
        /* Change the HBUS divider to its final value */
        imx233_clkctrl_set_div(CLK_HBUS, prof->hbus_div);
        /* Switch back CPU to PLL */
        imx233_clkctrl_set_bypass(CLK_CPU, false);
        /* Set the new EMI frequency */
        imx233_emi_set_frequency(prof->emi_freq);
    }
    else
    {
        /* Switch CPU to crystal at 24MHz */
        imx233_clkctrl_set_bypass(CLK_CPU, true);
        /* Program HBUS divider to its final value */
        imx233_clkctrl_set_div(CLK_HBUS, prof->hbus_div);
        /* Program CPU divider for PLL */
        imx233_clkctrl_set_frac_div(CLK_CPU, prof->cpu_fdiv);
        imx233_clkctrl_set_div(CLK_CPU, prof->cpu_idiv);
        /* Switch back CPU to PLL */
        imx233_clkctrl_set_bypass(CLK_CPU, false);
        /* Set the new EMI frequency */
        imx233_emi_set_frequency(prof->emi_freq);
        /* Change ARM cache timings */
        imx233_digctl_set_arm_cache_timings(prof->arm_cache_timings);
        /* Change VDDD regulator */
        imx233_power_set_regulator(REGULATOR_VDDD, prof->vddd, prof->vddd_bo);
    }
    /* enable auto slow again */
    imx233_clkctrl_enable_auto_slow(true);
    /* update frequency */
    cpu_frequency = frequency;
#else
    (void) frequency;
#endif
}

#ifdef HAVE_ADJUSTABLE_CPU_FREQ
void set_cpu_frequency(long frequency)
{
    return imx233_set_cpu_frequency(frequency);
}
#endif

void imx233_enable_usb_controller(bool enable)
{
    if(enable)
        BF_CLR(DIGCTL_CTRL, USB_CLKGATE);
    else
        BF_SET(DIGCTL_CTRL, USB_CLKGATE);
}

void imx233_enable_usb_phy(bool enable)
{
    if(enable)
    {
        BF_CLR(USBPHY_CTRL, SFTRST);
        BF_CLR(USBPHY_CTRL, CLKGATE);
        HW_USBPHY_PWD_CLR = 0xffffffff;
    }
    else
    {
        HW_USBPHY_PWD_SET = 0xffffffff;
        BF_SET(USBPHY_CTRL, SFTRST);
        BF_SET(USBPHY_CTRL, CLKGATE);
    }
}
