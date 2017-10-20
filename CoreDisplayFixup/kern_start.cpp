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

static NVRESL nvPatcherStart;

static void intelPatcherStart() {
	// apply corresopnding patches
	if (getKernelVersion() == KernelVersion::Yosemite ||          // 10.10.x
      getKernelVersion() == KernelVersion::ElCapitan) {         // 10.11.x
		lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));
	} else if (getKernelVersion() == KernelVersion::Sierra ||     // 10.12.x
             getKernelVersion() == KernelVersion::HighSierra) { // 10.13.x
		lilu.onProcLoad(ADDPR(procInfoSieHS), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSieHS), ADDPR(binaryModSize));
  }
}

static void cdfStart() {
	// check patcher disabling boot-args
	char tmp[16];
	bool bootargIntelOFF = PE_parse_boot_argn("-cdfinteloff", tmp, sizeof(tmp));
	bool bootargNVOFF    = PE_parse_boot_argn("-cdfnvoff", tmp, sizeof(tmp));
	
  if (! bootargIntelOFF) {
		intelPatcherStart();
  } else {
		DBGLOG("cdf", "IntelPatcher is disabled by kernel flag -cdfinteloff");
  }
	
  if (! bootargNVOFF) {
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
	KernelVersion::HighSierra,
  []() {
    cdfStart();
  }
};
