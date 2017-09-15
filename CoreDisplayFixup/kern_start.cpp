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

#include "IntelReslFixup.hpp"
#include "NVReslFixup.hpp"

static NVRESL nvresl;

// kernel versioning
static KernelVersion kernMajorVersion;
static KernelMinorVersion kernMinorVersion;
static int sysMajorVersion;
static int sysMinorVersion;

static void setSystemVersions() {
  kernMajorVersion = getKernelVersion();
  sysMajorVersion = kernMajorVersion - 4;
  
  kernMinorVersion = getKernelMinorVersion();
  // specific algogrithm to get kernel/system version of 10.12
  if (kernMajorVersion == KernelVersion::Sierra && kernMinorVersion > 2) // 10.12.x && 10.12.2+
    sysMinorVersion = kernMinorVersion - 1;
  else // older versions or 10.13+
    sysMinorVersion = kernMinorVersion;
}

static void intelStart() {
  DBGLOG("cdf @ IntelPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
  // apply corresopnding patches
  if (kernMajorVersion == KernelVersion::Yosemite ||      // 10.10.x
      kernMajorVersion == KernelVersion::ElCapitan)       // 10.11.x
    lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));
  else if (kernMajorVersion == KernelVersion::Sierra ||   // 10.12.x
           kernMajorVersion == KernelVersion::HighSierra) // 10.13.x
    lilu.onProcLoad(ADDPR(procInfoSieHS), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSieHS), ADDPR(binaryModSize));
  else  // unsupported
    SYSLOG("cdf @ loaded on unsupported macOS: 10.%d.%d", sysMajorVersion, sysMinorVersion);
}

static void nvStart() {
  DBGLOG("cdf @ NVPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
  nvresl.init();
}

static void cdfStart() {
  setSystemVersions();
  
  // check patcher disabling boot-args
  char tmp[16];
  bool bootargIntelOFF = PE_parse_boot_argn("-cdfinteloff", tmp, sizeof(tmp));
  bool bootargNVOFF    = PE_parse_boot_argn("-cdfnvoff", tmp, sizeof(tmp));
  
  if (!bootargIntelOFF)
    intelStart();
  else
    SYSLOG("cdf @ IntelPatcher is disabled by kernel flag \"-cdfinteloff\"");
  
  if (!bootargNVOFF)
    nvStart();
  else
    SYSLOG("cdf @ NVPatcher is disabled by kernel flag \"-cdfnvoff\"");
}

static const char *bootargOff[] = {
  "-cdfoff",
  "-x"
};

static const char *bootargDebug[] = {
  "-cdfdbg"
};

static const char *bootargBeta[] = {
  "-cdfbeta"
};

PluginConfiguration ADDPR(config) {
  xStringify(PRODUCT_NAME),
    
  // Lilu 1.1.0 and greater compatibility
  parseModuleVersion(xStringify(MODULE_VERSION)),
  
  // disabling args
  bootargOff,
  arrsize(bootargOff),

  // debug logging args
  bootargDebug,
  arrsize(bootargDebug),

  // enforcing args
  bootargBeta,
  arrsize(bootargBeta),
    
  // minKernel - 10.10
  KernelVersion::Yosemite,
  // maxKernel - 10.13
  KernelVersion::HighSierra,
  // let's start right now
  cdfStart
};
