//
//  NVPatcher.cpp
//  NVPatcher (CoreDisplayFixup)
//
//  Copyright © 2017 Vanilla. All rights reserved.
//
//  This module is based on lvs1974's NvidiaGraphicsFixup.
//

#include <Headers/kern_api.hpp>

#include "NVPatcher.hpp"

// NVDAGK100Hal.kext    - system built-in, for Kepler
static const char *kextGKHal[] = { "/System/Library/Extensions/NVDAGK100Hal.kext/Contents/MacOS/NVDAGK100Hal" }; // system embedded kext is always inside /S/L/E
static const char *kextGKHalId = "com.apple.nvidia.driver.NVDAGK100Hal";

// NVDAGK100HalWeb.kext - from web driver, for Kepler
static const char *kextGKWeb[] = {
	"/System/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb",
	"/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb"
};
static const char *kextGKWebId = "com.nvidia.web.NVDAGK100HalWeb";

// NVDAGM100HalWeb.kext - from web driver, for Maxwell
static const char *kextGMWeb[] = {
	"/System/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb",
	"/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb"
};
static const char *kextGMWebId = "com.nvidia.web.NVDAGM100HalWeb";

// NVDAGP100HalWeb.kext - from web driver, for Pascal
static const char *kextGPWeb[] = {
	"/System/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb",
	"/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb"
};
static const char *kextGPWebId = "com.nvidia.web.NVDAGP100HalWeb";

static KernelPatcher::KextInfo kextList[] {
	{ kextGKHalId, kextGKHal, arrsize(kextGKHal), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ kextGKWebId, kextGKWeb, arrsize(kextGKWeb), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ kextGMWebId, kextGMWeb, arrsize(kextGMWeb), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ kextGPWebId, kextGPWeb, arrsize(kextGPWeb), {}, {}, KernelPatcher::KextInfo::Unloaded }
};

static size_t kextListSize = arrsize(kextList);

bool NVPatcher::init()
{
	LiluAPI::Error error = lilu.onKextLoad(kextList, kextListSize, [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
		NVPatcher *nvpatcher = static_cast<NVPatcher *>(user);
		nvpatcher->processKext(patcher, index, address, size);
	}, this);
	
	if (error != LiluAPI::Error::NoError) {
		SYSLOG("NVPatcher", "failed to register onPatcherLoad method %d", error);
		return false;
	}
	
	return true;
}

void NVPatcher::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size)
{
  // Patches
  //
  // for NVDAGK100Hal and NVDAGK100HalWeb
  //
  // Reference:
  // https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-patcher.command
  //
  static const uint8_t gk100_find[] = { 0x88, 0x84, 0x02, 0x00 };
  static const uint8_t gk100_repl[] = { 0x80, 0x1A, 0x06, 0x00 };
  //
  // for NVDAGM100HalWeb and NVDAGP100HalWeb
  //
  // Reference:
  // https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-WEB-MAXWELL-patcher.command
  //
  static const uint8_t gmp100_find[] = { 0x88, 0x84, 0x02, 0x00 };
  static const uint8_t gmp100_repl[] = { 0x40, 0x42, 0x0F, 0x00 };

	if (progressState != ProcessingState::EverythingDone) {
		for (size_t i = 0; i < kextListSize; i++) {
			if (kextList[i].loadIndex == index) {
				DBGLOG("processKext", "current kext is %s progressState %d", kextList[i].id, progressState);
				
				// Patches for Kepler system built-in drivers (NVDAGK100Hal.kext)
				if (!(progressState & ProcessingState::NVGK100ReslPatched) && !strcmp(kextList[i].id, kextGKHalId)) {
					DBGLOG("NVPatcher", "found %s", kextGKHalId);
					// Ignore errors if the previous is undone
					patcher.clearError();
					KextPatch gk100_kext_patch {
						{ &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gk100_kext_patch, 1);
					progressState |= ProcessingState::NVGK100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextGKHalId);
				}
				
				// Patches for Kepler web driver (NVDAGK100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGK100WebReslPatched) && !strcmp(kextList[i].id, kextGKWebId)) {
					DBGLOG("NVPatcher", "found %s", kextGKWebId);
					KextPatch gk100web_kext_patch {
						{ &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gk100web_kext_patch, 1);
					progressState |= ProcessingState::NVGK100WebReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextGKWebId);
				}
				
				// Patches for Maxwell (NVDAGM100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGM100ReslPatched) && !strcmp(kextList[i].id, kextGMWebId)) {
					DBGLOG("NVPatcher", "found %s", kextGMWebId);
					KextPatch gm100_kext_patch {
						{ &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gm100_kext_patch, 1);
					progressState |= ProcessingState::NVGM100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextGMWebId);
				}
			
				// Patches for Pascal (NVDAGP100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGP100ReslPatched) && !strcmp(kextList[i].id, kextGPWebId)) {
					DBGLOG("NVPatcher", "found %s", kextGPWebId);
					KextPatch gp100_kext_patch {
						{ &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 2 }, // 2 occurrences need replacing here!
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gp100_kext_patch, 2); // 2 occurrences need replacing here!
					progressState |= ProcessingState::NVGP100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextGPWebId);
				}
			}
		}
	}
	// Ignore all the errors for other processors
	patcher.clearError();
}

void NVPatcher::applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchNum)
{
	DBGLOG("NVPatcher", "applying patches for %zu kext", index);
	for (size_t p = 0; p < patchNum; p++) {
		auto &patch = patches[p];
		if (patch.patch.kext->loadIndex == index) {
			if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
				DBGLOG("NVPatcher", "applying %zu patch for %zu kext", p, index);
				patcher.applyLookupPatch(&patch.patch);
				// Do not really care for the errors for now
				patcher.clearError();
			}
		}
	}
}
