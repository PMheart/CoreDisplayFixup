//
//  NVReslFixup.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 Vanilla. All rights reserved.
//
//  This kext is made based on lvs1974's NvidiaGraphicsFixup.
//

#include <Headers/kern_api.hpp>
#include <Headers/kern_util.hpp>
#include <Library/LegacyIOService.h>

#include <mach/vm_map.h>
#include <IOKit/IORegistryEntry.h>

#include "NVReslFixup.hpp"

// MARK:
// NVDAGK100Hal    - system built-in, might be for Kepler?
// NVDAGM100HalWeb - inside web driver, for Maxwell
// NVDAGP100HalWeb - inside web driver, for Pascal
static const char *kextNVDAGK100Hal[] {
  "/System/Library/Extensions/NVDAGK100Hal.kext/Contents/MacOS/NVDAGK100Hal"
};
static const char *kextNVDAGK100HalWeb[] {
  "/System/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb"
};
static const char *kextNVDAGM100HalWeb[] {
  "/System/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb"
};
static const char *kextNVDAGP100HalWeb[] {
  "/System/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb"
};

static KernelPatcher::KextInfo kextList[] {
  { "com.apple.nvidia.driver.NVDAGK100Hal", kextNVDAGK100Hal, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  { "com.nvidia.web.NVDAGK100HalWeb", kextNVDAGK100HalWeb, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  { "com.nvidia.web.NVDAGM100HalWeb", kextNVDAGM100HalWeb, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  { "com.nvidia.web.NVDAGP100HalWeb", kextNVDAGP100HalWeb, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
};

static size_t kextListSize {4};

// Patches
// for NVDAGK100Hal and NVDAGK100HalWeb
//
// Reference:
// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-patcher.command
//
static const uint8_t gk100_find[] = { 0x88, 0x84, 0x02, 0x00 };
static const uint8_t gk100_repl[] = { 0x80, 0x1A, 0x06, 0x00 };
// for NVDAGM100HalWeb and NVDAGP100HalWeb
//
// Reference:
// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-WEB-MAXWELL-patcher.command
//
static const uint8_t gmp100_find[] = { 0x88, 0x84, 0x02, 0x00 };
static const uint8_t gmp100_repl[] = { 0x00, 0x35, 0x0C, 0x00 };

bool NVRESL::init() {
  LiluAPI::Error error = lilu.onKextLoad(kextList, kextListSize,
                                         [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
                                           NVRESL *nvresl = static_cast<NVRESL *>(user);
                                           nvresl->processKext(patcher, index, address, size);
                                         }, this);
  if (error != LiluAPI::Error::NoError) {
    SYSLOG("cdf @ NVPatcher failed to register onPatcherLoad method %d", error);
    return false;
  }
  
  return true;
}

void NVRESL::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
  if (progressState != ProcessingState::EverythingDone) {
    for (size_t i = 0; i < kextListSize; i++) {
      if (kextList[i].loadIndex == index) {
        // Patches for Kepler system built-in drivers
        if (!(progressState & ProcessingState::NVGK100ReslPatched) && !strcmp(kextList[i].id, "com.apple.nvidia.driver.NVDAGK100Hal")) {
          DBGLOG("cdf @ NVPatcher found com.apple.nvidia.driver.NVDAGK100Hal");

          KextPatch gk100_kext_patch {
            { &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal");
          //break;
        }
        
        // Patches for Kepler web driver
        if (!(progressState & ProcessingState::NVGK100WebReslPatched) && !strcmp(kextList[i].id, "com.nvidia.web.NVDAGK100HalWeb")) {
          DBGLOG("cdf @ NVPatcher found com.nvidia.web.NVDAGK100HalWeb");
          
          KextPatch gk100web_kext_patch {
            { &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100web_kext_patch, 1);
          progressState |= ProcessingState::NVGK100WebReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100HalWeb");
          //break;
        }
        
        // Patches for Maxwell
        if (!(progressState & ProcessingState::NVGM100ReslPatched) && !strcmp(kextList[i].id, "com.nvidia.web.NVDAGM100HalWeb")) {
          DBGLOG("cdf @ NVPatcher found com.nvidia.web.NVDAGM100HalWeb");
        
          KextPatch gm100_kext_patch {
            { &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gm100_kext_patch, 1);
          progressState |= ProcessingState::NVGM100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGM100HalWeb");
          //break;
        }
      
        // Patches for Pascal
        if (!(progressState & ProcessingState::NVGP100ReslPatched) && !strcmp(kextList[i].id, "com.nvidia.web.NVDAGP100HalWeb")) {
          DBGLOG("cdf @ NVPatcher found com.nvidia.web.NVDAGP100HalWeb");
        
          KextPatch gp100_kext_patch {
            { &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 2 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gp100_kext_patch, 1);
          progressState |= ProcessingState::NVGP100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGP100HalWeb");
          //break;
        }
      }
    }
  }
  
  // Ignore all the errors for other processors
  patcher.clearError();
}

void NVRESL::applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchNum) {
  DBGLOG("cdf @ NVPatcher applying patches for %zu kext", index);
  for (size_t p = 0; p < patchNum; p++) {
    auto &patch = patches[p];
    if (patch.patch.kext->loadIndex == index) {
      if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
        DBGLOG("cdf @ NVPatcher applying %zu patch for %zu kext", p, index);
        patcher.applyLookupPatch(&patch.patch);
        // Do not really care for the errors for now
        patcher.clearError();
      }
    }
  }
}
