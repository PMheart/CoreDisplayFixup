//
//  kern_start.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

//
// Lilu::Headers
//
#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>


//
// CoreDisplayFixup::Headers
//
#include "kern_cdPatch.hpp"


//
// The version of this kext.
//
#define kextVer "1.2.1"


//
// The main function of this kext.
//
static void main()
{
    //
    // The temporary workaround of lack of OS Dependency feature in <Headers/kern_user.hpp>,
    // patching the target framework(s) based on darwin version.
    //
    // Cons:
    //   * The patch(es) can be catastrophic once there will be different patches in the same major version.
    //   * More things are to be updated.
    //
    KernelVersion kernMajorVer = getKernelVersion();
    
    //
    // Notes:
    //   * SYSLOG("Message"); will print "<Plug-in name>: Message".
    //   * DBGLOG("Message"); will print "<Plug-in name>: (DEBUG) Message".
    //
    SYSLOG("Version %s loaded. Copyright (c) 2017 vit9696, Vanilla. All rights reserved.", kextVer);
    
    //
    // Patch(es) for Yosemite. (10.10, Darwin 14)
    //
    if (kernMajorVer == KernelVersion::Yosemite)
    {
        lilu.onProcLoad
        (
            ADDPR(procInfoYosemite),
            ADDPR(procInfoSize),
            nullptr,
            nullptr,
            ADDPR(binaryModYosemite),
            ADDPR(binaryModSize)
        );
        
        DBGLOG("IOKit 10.%d (Darwin %d) found and patched.", kernMajorVer - 4, kernMajorVer);
    }
    
    //
    // Patch(es) for El Capitan. (10.11, Darwin 15)
    //
    if (kernMajorVer == KernelVersion::ElCapitan)
    {
        lilu.onProcLoad
        (
            ADDPR(procInfoCapitan),
            ADDPR(procInfoSize),
            nullptr,
            nullptr,
            ADDPR(binaryModCapitan),
            ADDPR(binaryModSize)
        );
        
        DBGLOG("IOKit 10.%d (Darwin %d) found and patched.", kernMajorVer - 4, kernMajorVer);
    }
    
    //
    // Patch(es) for Sierra. (10.12, Darwin 16)
    //
    if (kernMajorVer == KernelVersion::Sierra)
    {
        lilu.onProcLoad
        (
            ADDPR(procInfoSierra),
            ADDPR(procInfoSize),
            nullptr,
            nullptr,
            ADDPR(binaryModSierra),
            ADDPR(binaryModSize)
        );
        
        DBGLOG("CoreDisplay 10.%d (Darwin %d) found and patched.", kernMajorVer - 4, kernMajorVer);
    }
}

//
// Boot argument to unload this kext.
//
static const char *bootargOff[]
{
    "-cdfoff"
};

//
// Boot argument to enable debug logging of this kext. [ DBGLOG("Message"); // will be called]
//
static const char *bootargDebug[]
{
    "-cdfdbg"
};

//
// Boot argument to load this kext on an unsupported darwin version.
//
static const char *bootargBeta[]
{
    "-cdfbeta"
};

//
// Start calling Lilu. (Check <Headers/plugin_start.hpp>)
//
PluginConfiguration ADDPR(config)
{
    //
    // The name of the Lilu plug-in (CoreDisplayFixup here).
    //
    xStringify(PRODUCT_NAME),
    //
    // For Lilu 1.1.0 and above, comment out this will bring the compatibility of older Lilu (1.0.0)
    //
    parseModuleVersion(xStringify(MODULE_VERSION)),
    //
    // Check whether unloading argument(s) is (are) used.
    //
    bootargOff,
    sizeof(bootargOff)/sizeof(bootargOff[0]),
    //
    // Check whether debugging argument(s) is (are) used.
    //
    bootargDebug,
    sizeof(bootargDebug)/sizeof(bootargDebug[0]),
    //
    // Check whether enforcing argument(s) is (are) used.
    //
    bootargBeta,
    sizeof(bootargBeta)/sizeof(bootargBeta[0]),
    //
    // The minimum darwin version to load this kext. (Should be 10.10 - darwin 14.)
    //
    KernelVersion::Yosemite,
    //
    // The maximum darwin version to load this kext. (Should be 10.12 - darwin 16.)
    //
    KernelVersion::Sierra,
    //
    // Start calling the function main() of this kext.
    //
    main
};
