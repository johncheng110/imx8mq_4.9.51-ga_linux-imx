/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 * Copyright 2017 NXP.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/irqchip.h>
#include <linux/of_platform.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "common.h"
#include "cpuidle.h"
#include "hardware.h"

/* static IO mapping, and ioremap() could always share the same mapping. */
static struct map_desc mx7ulp_io_desc[] __initdata = {
	mx7ulp_aips_map_entry(1, MT_DEVICE),
	mx7ulp_aips_map_entry(2, MT_DEVICE),
	mx7ulp_aips_map_entry(3, MT_DEVICE),
	mx7ulp_aips_map_entry(4, MT_DEVICE),
	mx7ulp_aips_map_entry(5, MT_DEVICE),
};

static void __init imx7ulp_init_machine(void)
{
	struct device *parent;

	parent = imx_soc_device_init();
	if (parent == NULL)
		pr_warn("failed to initialize soc device\n");

	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static void __init imx7ulp_init_irq(void)
{
	/* TBD */
	mxc_set_cpu_type(MXC_CPU_IMX7ULP);

	irqchip_init();
	imx7ulp_pm_init();
}

static void __init imx7ulp_map_io(void)
{
	iotable_init(mx7ulp_io_desc, ARRAY_SIZE(mx7ulp_io_desc));
	imx7ulp_pm_map_io();
}

static void __init imx7ulp_init_late(void)
{
	if (IS_ENABLED(CONFIG_ARM_IMX7ULP_CPUFREQ))
		platform_device_register_simple("imx7ulp-cpufreq", -1, NULL, 0);

	imx7ulp_cpuidle_init();
	imx7ulp_enable_nmi();
}

static const char *const imx7ulp_dt_compat[] __initconst = {
	"fsl,imx7ulp",
	NULL,
};

DT_MACHINE_START(IMX7ulp, "Freescale i.MX7ULP (Device Tree)")
	.map_io		= imx7ulp_map_io,
	.init_irq	= imx7ulp_init_irq,
	.init_machine	= imx7ulp_init_machine,
	.init_late      = imx7ulp_init_late,
	.dt_compat	= imx7ulp_dt_compat,
MACHINE_END
