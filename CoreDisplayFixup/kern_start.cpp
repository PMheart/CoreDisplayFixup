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

#include "PrivateAPI.h"
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
  if (kernMajorVersion == KernelVersion::Sierra && kernMinorVersion > 2) // 10.12.x && 10.12.2+
    sysMinorVersion = kernMinorVersion - 1;
  else // older versions or 10.13+
    sysMinorVersion = kernMinorVersion;
}

static void intelStart() {
  SYSLOG("cdf @ IntelPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
    
  if (kernMajorVersion == KernelVersion::Yosemite || kernMajorVersion == KernelVersion::ElCapitan) // if 10.10.x or 10.11.x
    lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));
  else if (kernMajorVersion == KernelVersion::Sierra || kernMajorVersion == KernelVersion::HighSierra) // if 10.12.x or 10.13.x
    lilu.onProcLoad(ADDPR(procInfoSieHigh), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSieHigh), ADDPR(binaryModSize));
  else
    SYSLOG("cdf @ loaded on unsupported macOS: 10.%d.%d", sysMajorVersion, sysMinorVersion);
}

static void nvStart() {
  SYSLOG("cdf @ NVPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
  nvresl.init();
}

static void cdfStart() {
  setSystemVersions();
  
  // check disabling boot-args
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
  "-cdfoff"
};

static const char *bootargDebug[] = {
  "-cdfdbg"
};

static const char *bootargBeta[] = {
  "-cdfbeta"
};

PluginConfiguration ADDPR(config)
{
  xStringify(PRODUCT_NAME),
    
  // Lilu 1.1.0 and greater compatibility
  parseModuleVersion(xStringify(MODULE_VERSION)),
    
  bootargOff,
  arrsize(bootargOff),

  bootargDebug,
  arrsize(bootargDebug),

  bootargBeta,
  arrsize(bootargBeta),
    
  // minKernel - 10.10
  KernelVersion::Yosemite,
  // maxKernel - 10.13
  KernelVersion::HighSierra,
    
  cdfStart
};
