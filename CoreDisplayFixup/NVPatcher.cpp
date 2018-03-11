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
// Note: system embedded kext is always inside /S/L/E
static const char *kextGKHal[] = { "/System/Library/Extensions/NVDAGK100Hal.kext/Contents/MacOS/NVDAGK100Hal" };

// NVDAGK100HalWeb.kext - from web driver, for Kepler
static const char *kextGKWeb[] = {
	"/System/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb",
	"/Library/Extensions/NVDAGK100HalWeb.kext/Contents/MacOS/NVDAGK100HalWeb"
};

// NVDAGM100HalWeb.kext - from web driver, for Maxwell
static const char *kextGMWeb[] = {
	"/System/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb",
	"/Library/Extensions/NVDAGM100HalWeb.kext/Contents/MacOS/NVDAGM100HalWeb"
};

// NVDAGP100HalWeb.kext - from web driver, for Pascal
static const char *kextGPWeb[] = {
	"/System/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb",
	"/Library/Extensions/NVDAGP100HalWeb.kext/Contents/MacOS/NVDAGP100HalWeb"
};

static KernelPatcher::KextInfo kextList[] {
	{ "com.apple.nvidia.driver.NVDAGK100Hal", kextGKHal, arrsize(kextGKHal), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ "com.nvidia.web.NVDAGK100HalWeb", kextGKWeb, arrsize(kextGKWeb), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ "com.nvidia.web.NVDAGM100HalWeb", kextGMWeb, arrsize(kextGMWeb), {}, {}, KernelPatcher::KextInfo::Unloaded },
	{ "com.nvidia.web.NVDAGP100HalWeb", kextGPWeb, arrsize(kextGPWeb), {}, {}, KernelPatcher::KextInfo::Unloaded }
};

enum : size_t {
	KextGK100HalSys,
	KextGK100HalWeb,
	KextGM100HalWeb,
	KextGP100HalWeb
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
				if (!(progressState & ProcessingState::NVGK100ReslPatched) && i == KextGK100HalSys) {
					DBGLOG("NVPatcher", "found %s", kextList[i].id);
					// Ignore errors if the previous is undone
					patcher.clearError();
					KextPatch gk100_kext_patch {
						{ &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gk100_kext_patch, 1);
					progressState |= ProcessingState::NVGK100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextList[i].id);
				}
				
				// Patches for Kepler web driver (NVDAGK100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGK100WebReslPatched) && i == KextGK100HalWeb) {
					DBGLOG("NVPatcher", "found %s", kextList[i].id);
					KextPatch gk100web_kext_patch {
						{ &kextList[i], gk100_find, gk100_repl, sizeof(gk100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gk100web_kext_patch, 1);
					progressState |= ProcessingState::NVGK100WebReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextList[i].id);
				}
				
				// Patches for Maxwell (NVDAGM100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGM100ReslPatched) && i == KextGM100HalWeb) {
					DBGLOG("NVPatcher", "found %s", kextList[i].id);
					KextPatch gm100_kext_patch {
						{ &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 1 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gm100_kext_patch, 1);
					progressState |= ProcessingState::NVGM100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextList[i].id);
				}
			
				// Patches for Pascal (NVDAGP100HalWeb.kext)
				if (!(progressState & ProcessingState::NVGP100ReslPatched) && i == KextGP100HalWeb) {
					DBGLOG("NVPatcher", "found %s", kextList[i].id);
					KextPatch gp100_kext_patch {
						 // 2 occurrences need replacing here!
						{ &kextList[i], gmp100_find, gmp100_repl, sizeof(gmp100_find), 2 },
						KernelVersion::MountainLion, KernelVersion::HighSierra
					};
					applyPatches(patcher, index, &gp100_kext_patch, 1);
					progressState |= ProcessingState::NVGP100ReslPatched;
					DBGLOG("NVPatcher", "patched %s", kextList[i].id);
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
