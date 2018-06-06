//
//  kern_start.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "IntelPatcher.hpp"
#include "NVPatcher.hpp"

static NVPatcher nvPatcherStart;

static void intelPatcherStart()
{
	// apply corresopnding patches
	if (getKernelVersion() == KernelVersion::Yosemite || getKernelVersion() == KernelVersion::ElCapitan) // 10.10, 10.11
		lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModOldSize));
	else if (getKernelVersion() == KernelVersion::Sierra || (getKernelVersion() == KernelVersion::HighSierra && getKernelMinorVersion() < 5)) // 10.12, 10.13.0-10.13.3
		lilu.onProcLoad(ADDPR(procInfoSieHS), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSieHS), ADDPR(binaryModOldSize));
	else if ((getKernelVersion() == KernelVersion::HighSierra && getKernelMinorVersion() >= 5) || getKernelVersion() == KernelVersion::Mojave) // 10.13.4+
		lilu.onProcLoad(ADDPR(procInfoSieHS), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModHS1034), ADDPR(binaryModHS1034Size));
}

static void cdfStart()
{
	// check patcher disabling boot-args
	char tmp[16];
	bool bootargIntelOFF = PE_parse_boot_argn("-cdfinteloff", tmp, sizeof(tmp));
	bool bootargNVOFF    = PE_parse_boot_argn("-cdfnvoff", tmp, sizeof(tmp));

	if (!bootargIntelOFF) {
		intelPatcherStart();
	} else {
		DBGLOG("cdf", "IntelPatcher is disabled by kernel flag -cdfinteloff");
	}

	if (!bootargNVOFF) {
		nvPatcherStart.init();
	} else {
		DBGLOG("cdf", "NVPatcher is disabled by kernel flag -cdfnvoff");
	}
}

static const char *bootargOff[] = {
	"-cdfoff",
};

static const char *bootargDebug[] = {
	"-cdfdbg"
};

static const char *bootargBeta[] = {
	"-cdfbeta"
};

PluginConfiguration ADDPR(config) {
	xStringify(PRODUCT_NAME),
	parseModuleVersion(xStringify(MODULE_VERSION)),
	LiluAPI::AllowNormal | LiluAPI::AllowInstallerRecovery,
	bootargOff,
	arrsize(bootargOff),
	bootargDebug,
	arrsize(bootargDebug),
	bootargBeta,
	arrsize(bootargBeta),
	KernelVersion::Yosemite,
	KernelVersion::Mojave,
	[]() {
		cdfStart();
	}
};
