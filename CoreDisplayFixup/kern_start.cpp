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

KernelVersion kernMajorVersion = getKernelVersion();
static NVRESL nvresl;
extern "C" int version_minor;

int getSystemMinorVersion() {
  int sysMinorVersion = 0;
    
  if (kernMajorVersion == KernelVersion::Sierra && version_minor > 2) // 10.12.2+
    sysMinorVersion = version_minor - 1;
  else // legacy versions
    sysMinorVersion = version_minor;
    
  return sysMinorVersion;
}

// kernel versioning
int sysMajorVersion = kernMajorVersion - 4;
int kernMinorVersion = getSystemMinorVersion();
int sysMinorVersion = kernMinorVersion;

static void intelStart() {
  SYSLOG("cdf @ IntelPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
    
  if (kernMajorVersion == KernelVersion::Yosemite || kernMajorVersion == KernelVersion::ElCapitan) // if 10.10.x or 10.11.x
    lilu.onProcLoad(ADDPR(procInfoYosEC), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModYosEC), ADDPR(binaryModSize));
  else if (kernMajorVersion == KernelVersion::Sierra) // if 10.12.x
    lilu.onProcLoad(ADDPR(procInfoSie), ADDPR(procInfoSize), nullptr, nullptr, ADDPR(binaryModSie), ADDPR(binaryModSize));
}

static void nvStart() {
  SYSLOG("cdf @ NVPatcher starting on macOS 10.%d.%d", sysMajorVersion, sysMinorVersion);
    
  nvresl.init();
}

static void cdfStart() {

  // check boot-args
  char tmp[16];
  bool bootargIntelOFF = PE_parse_boot_argn("-cdfinteloff", tmp, sizeof(tmp));
  bool bootargNVOFF    = PE_parse_boot_argn("-cdfnvoff", tmp, sizeof(tmp));
  
  if (!bootargIntelOFF)
    intelStart();
  
  if (!bootargNVOFF)
    nvStart();
}

static const char *bootargOff[] = {
  "-cdfoff"
  // add more args here...
};

static const char *bootargDebug[] = {
  "-cdfdbg"
  // add more args here...
};

static const char *bootargBeta[] = {
  "-cdfbeta"
  // add more args here...
};

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
    
  // minKernel - 10.10
  KernelVersion::Yosemite,
  // maxKernel - 10.12
  KernelVersion::Sierra,
    
  cdfStart
};
