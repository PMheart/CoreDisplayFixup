//
//  kern_start.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
// This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "kern_cdPatch.hpp"

void cdFixupStart() {
    DBGLOG("cdf @ loaded");
  
    // temp workaround of lack of OS Dependency
    KernelVersion kernMajorVer = getKernelVersion();
    
    DBGLOG("cdf @ calling Lilu for determining target Kernel Major Version: %d", kernMajorVer);
    
    if (kernMajorVer == KernelVersion::Yosemite || kernMajorVer == KernelVersion::ElCapitan) // 10.10.x or 10.11.x
        lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));      // Lilu called
    else // 10.12.x
        lilu.onProcLoad(ADDPR(procInfoSie), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSie), ADDPR(binaryModSize));    // Lilu called
}

const char *bootargOff[] { // unload this kext anyway
    "-cdfoff"
};

const char *bootargDebug[] { // enable debug logging (enable DBGLOG();)
    "-cdfdbg"
};

const char *bootargBeta[] { // load this kext anyway
    "-cdfbeta"
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    bootargOff,
    sizeof(bootargOff)/sizeof(bootargOff[0]),
    bootargDebug,
    sizeof(bootargDebug)/sizeof(bootargDebug[0]),
    bootargBeta,
    sizeof(bootargBeta)/sizeof(bootargBeta[0]),
    KernelVersion::Yosemite,       // minKernel - 10.10.x (darwin 14)
    KernelVersion::Sierra,         // maxKernel - 10.12.x (darwin 16)
    cdFixupStart
};
