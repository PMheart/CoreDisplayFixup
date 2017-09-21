//
//  NVReslFixup.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 Vanilla. All rights reserved.
//
//  This kext is made based on lvs1974's NvidiaGraphicsFixup.
//

#include <Headers/kern_api.hpp>

#include "NVReslFixup.hpp"

// macros for following arrays
#define kGK100org 0
#define kGK100web 1
#define kGM100web 2
#define kGP100web 3

// MARK:
// NVDAGK100Hal    - system built-in,   for Kepler
// NVDAGK100HalWeb - inside web driver, for Kepler
// NVDAGM100HalWeb - inside web driver, for Maxwell
// NVDAGP100HalWeb - inside web driver, for Pascal
static const char *idList[] {
  "com.apple.nvidia.driver.NVDAGK100Hal",
  "com.nvidia.web.NVDAGK100HalWeb",
  "com.nvidia.web.NVDAGM100HalWeb",
  "com.nvidia.web.NVDAGP100HalWeb"
};

static const char *binList[] {
  "/System/Library/Extensions/NVDAGK100Hal.kext/Contents/MacOS/NVDAGK100Hal",
  "/System/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb",
  "/System/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb",
  "/System/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb"
};

static KernelPatcher::KextInfo kextList[] {
  { idList[kGK100org], &binList[kGK100org], 1, {true}, {}, KernelPatcher::KextInfo::Unloaded },
  { idList[kGK100web], &binList[kGK100web], 1, {true}, {}, KernelPatcher::KextInfo::Unloaded },
  { idList[kGM100web], &binList[kGM100web], 1, {true}, {}, KernelPatcher::KextInfo::Unloaded },
  { idList[kGP100web], &binList[kGP100web], 1, {true}, {}, KernelPatcher::KextInfo::Unloaded },
};

static size_t kextListSize = arrsize(kextList);

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
  
  if (progressState != ProcessingState::EverythingDone) {
    for (size_t i = 0; i < kextListSize; i++) {
      if (kextList[i].loadIndex == index) {
        // Patches for Kepler system built-in drivers
        if (!(progressState & ProcessingState::NVGK100ReslPatched) && !strcmp(kextList[i].id, idList[kGK100org])) {
          DBGLOG("cdf @ NVPatcher found %s", idList[kGK100org]);
          patcher.clearError();
          KextPatch gk100_kext_patch {
            { &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
            // FIXME: revert KernelPatcher::KernelAny back to KernelVersion::HighSierra
          //KernelVersion::MountainLion, KernelVersion::HighSierra
            KernelVersion::MountainLion, KernelPatcher::KernelAny
          };
          applyPatches(patcher, index, &gk100_kext_patch, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal");
        }
        
        // Patches for Kepler web driver
        if (!(progressState & ProcessingState::NVGK100WebReslPatched) && !strcmp(kextList[i].id, idList[kGK100web])) {
          DBGLOG("cdf @ NVPatcher found %s", idList[kGK100web]);
          patcher.clearError();
          KextPatch gk100web_kext_patch {
            { &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
            // FIXME: revert KernelPatcher::KernelAny back to KernelVersion::HighSierra
          //KernelVersion::MountainLion, KernelVersion::HighSierra
            KernelVersion::MountainLion, KernelPatcher::KernelAny
          };
          applyPatches(patcher, index, &gk100web_kext_patch, 1);
          progressState |= ProcessingState::NVGK100WebReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100HalWeb");
        }
        
        // Patches for Maxwell
        if (!(progressState & ProcessingState::NVGM100ReslPatched) && !strcmp(kextList[i].id, idList[kGM100web])) {
          DBGLOG("cdf @ NVPatcher found %s", idList[kGM100web]);
          patcher.clearError();
          KextPatch gm100_kext_patch {
            { &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 1 },
            // FIXME: revert KernelPatcher::KernelAny back to KernelVersion::HighSierra
          //KernelVersion::MountainLion, KernelVersion::HighSierra
            KernelVersion::MountainLion, KernelPatcher::KernelAny
          };
          applyPatches(patcher, index, &gm100_kext_patch, 1);
          progressState |= ProcessingState::NVGM100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGM100HalWeb");
        }
      
        // Patches for Pascal
        if (!(progressState & ProcessingState::NVGP100ReslPatched) && !strcmp(kextList[i].id, idList[kGP100web])) {
          DBGLOG("cdf @ NVPatcher found %s", idList[kGP100web]);
          patcher.clearError();
          KextPatch gp100_kext_patch {
            { &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 2 }, // 2 occurrences to be replaced here!
            // FIXME: revert KernelPatcher::KernelAny back to KernelVersion::HighSierra
          //KernelVersion::MountainLion, KernelVersion::HighSierra
            KernelVersion::MountainLion, KernelPatcher::KernelAny
          };
          applyPatches(patcher, index, &gp100_kext_patch, 1);
          progressState |= ProcessingState::NVGP100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGP100HalWeb");
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
