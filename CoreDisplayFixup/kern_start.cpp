//
//    kern_start.cpp
//    CoreDisplayFixup
//
//    Created by Vanilla on 3/26/17.
//    Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//    This kext is made based on vit9696's Shiki, without his amazing repo the repo won't be here!
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>

#include "kern_cdPatch.hpp"

void cdFixupStart() {
    DBGLOG("cdf @ loaded");
  
    // temp workaround of lack of OS Dependency
    KernelVersion kernMajorVer = getKernelVersion();
    
    DBGLOG("cdf @ calling Lilu for determining target Kernel Major Version: %d", kernMajorVer);
    
    if (kernMajorVer == KernelVersion::Yosemite)
        lilu.onProcLoad(ADDPR(procInfoYosemite), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosemite), ADDPR(binaryModSize));
    else if (kernMajorVer == KernelVersion::ElCapitan)
          lilu.onProcLoad(ADDPR(procInfoCapitan), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModCapitan), ADDPR(binaryModSize));
    else
          lilu.onProcLoad(ADDPR(procInfoSierra), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSierra), ADDPR(binaryModSize));
}

const char *bootargOff[] {
    "-cdfoff"
};

const char *bootargDebug[] {
    "-cdfdbg"
};

const char *bootargBeta[] {
    "-cdfbeta"
};

PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),     // for Lilu 1.1.0 and above
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
