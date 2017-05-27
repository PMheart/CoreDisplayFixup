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

#include "CoreDisplayFixup.hpp"
#include "OSMinorVersion.hpp"

static void cdfStart() {
    OSVersion *osVersion = new OSVersion();
    KernelVersion kernMajorVer = getKernelVersion();
    
    SYSLOG("cdf @ Starting on macOS 10.%d.%d", kernMajorVer - 4, osVersion->getMinorVersion());
    
    if (kernMajorVer == KernelVersion::Yosemite || kernMajorVer == KernelVersion::ElCapitan) // if 10.10.x or 10.11.x
        lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));
    else if (kernMajorVer == KernelVersion::Sierra) // if 10.12.x
        lilu.onProcLoad(ADDPR(procInfoSie), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSie), ADDPR(binaryModSize));
    else // currently unsupported
        SYSLOG("cdf @ Loading on unsupported OS");
}

// kext flag to unload CoreDisplayFixup
static const char *bootargOff[] = { "-cdfoff" };

// kext flag to enable debug logging of CoreDisplayFixup
// (only available while CDF is accompanied by debug build Lilu)
static const char *bootargDebug[] = { "-cdfdbg" };

// kext flag to load CoreDisplayFixup regardless of darwin version
static const char *bootargBeta[] = { "-cdfbeta" };

PluginConfiguration ADDPR(config)
{
    xStringify(PRODUCT_NAME),
    
    // Lilu 1.1.0 and greater compatibility
    parseModuleVersion(xStringify(MODULE_VERSION)),
    
    bootargOff,
    sizeof(bootargOff)/sizeof(bootargOff[0]),

    bootargDebug,
    sizeof(bootargDebug)/sizeof(bootargDebug[0]),

    bootargBeta,
    sizeof(bootargBeta)/sizeof(bootargBeta[0]),
    
    // minKernel
    KernelVersion::Yosemite,
    // maxKernel
    KernelVersion::Sierra,
    
    cdfStart
};
