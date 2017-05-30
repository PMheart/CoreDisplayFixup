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
static const char *kextNVDAGM100HalWeb[] {
  "/System/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb"
};
static const char *kextNVDAGP100HalWeb[] {
  "/System/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb"
};

static KernelPatcher::KextInfo kextList[] {
  { "com.apple.nvidia.driver.NVDAGK100Hal", kextNVDAGK100Hal, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  { "com.nvidia.web.NVDAGM100HalWeb", kextNVDAGM100HalWeb, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  { "com.nvidia.web.NVDAGP100HalWeb", kextNVDAGP100HalWeb, 1, true, {}, KernelPatcher::KextInfo::Unloaded },
  
};

static size_t kextListSize {3};

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
        // Patches for Kepler
        if (!(progressState & ProcessingState::NVGK100ReslPatched) && strcmp(kextList[i].id, "com.apple.nvidia.driver.NVDAGK100Hal")) {
          DBGLOG("cdf @ NVPatcher found com.apple.nvidia.driver.NVDAGK100Hal");
        
          //
          // Reference:
          // https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-patcher.command
          //
        
          // patch #1-1 start
          const uint8_t gk100_find1_1[] = { 0xC7, 0x82, 0xC8, 0x00, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gk100_repl1_1[] = { 0xC7, 0x82, 0xC8, 0x00, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
      
          KextPatch gk100_kext_patch_1_1 {
            { &kextList[i], gk100_find1_1, gk100_repl1_1, sizeof(gk100_find1_1), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch_1_1, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 1-1");
          // patch #1-1 end
        
          // patch #1-2 start
          const uint8_t gk100_find1_2[] = { 0xC7, 0x82, 0x10, 0x01, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gk100_repl1_2[] = { 0xC7, 0x82, 0x10, 0x01, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
        
          KextPatch gk100_kext_patch_1_2 {
            { &kextList[i], gk100_find1_2, gk100_repl1_2, sizeof(gk100_find1_2), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch_1_2, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 1-2");
          // patch #1-2 end
        
          // patch #2-1 start
          const uint8_t gk100_find2_1[] = { 0xC7, 0x82, 0xD0, 0x00, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gk100_repl2_1[] = { 0xC7, 0x82, 0xD0, 0x00, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
        
          KextPatch gk100_kext_patch_2_1 {
            { &kextList[i], gk100_find2_1, gk100_repl2_1, sizeof(gk100_find2_1), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch_2_1, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 2-1");
          // patch #2-1 end
        
          // patch #2-2 start
          const uint8_t gk100_find2_2[] = { 0xC7, 0x82, 0x20, 0x01, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gk100_repl2_2[] = { 0xC7, 0x82, 0x20, 0x01, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
        
          KextPatch gk100_kext_patch_2_2 {
            { &kextList[i], gk100_find2_2, gk100_repl2_2, sizeof(gk100_find2_2), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch_2_2, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 2-2");
          // patch #2-2 end
        
          // patch #3 start
          const uint8_t gk100_find3[] = { 0x8B, 0x82, 0xD0, 0x00, 0x00, 0x00, 0xB9, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gk100_repl3[] = { 0x8B, 0x82, 0xD0, 0x00, 0x00, 0x00, 0xB9, 0x80, 0x1A, 0x06, 0x00 };
        
          KextPatch gk100_kext_patch_3 {
            { &kextList[i], gk100_find3, gk100_repl3, sizeof(gk100_find3), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gk100_kext_patch_3, 1);
          progressState |= ProcessingState::NVGK100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 3");
          // patch #3 end
        }
      
        // Patches for Maxwell
        if (!(progressState & ProcessingState::NVGM100ReslPatched) && strcmp(kextList[i].id, "com.nvidia.web.NVDAGM100HalWeb")) {
          DBGLOG("cdf @ NVPatcher found com.nvidia.web.NVDAGM100HalWeb");
        
          //
          // Reference:
          // https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-WEB-MAXWELL-patcher.command
          //
        
          // patch #1 start
          const uint8_t gm100_find1[] = { 0x8B, 0x82, 0xB0, 0x00, 0x00, 0x00, 0xB9, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gm100_repl1[] = { 0x8B, 0x82, 0xB0, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x35, 0x0C, 0x00 };
        
          KextPatch gm100_kext_patch_1 {
            { &kextList[i], gm100_find1, gm100_repl1, sizeof(gm100_find1), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gm100_kext_patch_1, 1);
          progressState |= ProcessingState::NVGM100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGM100HalWeb version 1");
          // patch #1 end
        }
      
        // Patches for Pascal
        if (!(progressState & ProcessingState::NVGM100ReslPatched) && strcmp(kextList[i].id, "com.nvidia.web.NVDAGP100HalWeb")) {
          DBGLOG("cdf @ NVPatcher found com.nvidia.web.NVDAGP100HalWeb");
        
          //
          // Reference:
          // https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-WEB-PASCAL-patcher.command
          //
        
          // patch #1 start
          const uint8_t gp100_find1[] = { 0x8B, 0x82, 0xB0, 0x00, 0x00, 0x00, 0xB9, 0x88, 0x84, 0x02, 0x00 };
          const uint8_t gp100_repl1[] = { 0x8B, 0x82, 0xB0, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x35, 0x0C, 0x00 };
        
          KextPatch gp100_kext_patch_1 {
            { &kextList[i], gp100_find1, gp100_repl1, sizeof(gp100_find1), 1 },
            KernelVersion::MountainLion, KernelVersion::Sierra
          };
          applyPatches(patcher, index, &gp100_kext_patch_1, 1);
          progressState |= ProcessingState::NVGP100ReslPatched;
          DBGLOG("cdf @ NVPatcher patched NVDAGP100HalWeb version 1");
          // patch #1 end
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
