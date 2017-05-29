//
//  NVReslFixup.cpp
//  CoreDisplayFixup
//
//  Created by Vanilla on 5/28/17.
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

static const char *kextNVDAGK100Hal[] {
	"/System/Library/Extensions/NVDAGK100Hal.kext/Contents/MacOS/NVDAGK100Hal"
};

static KernelPatcher::KextInfo kextList[] {
	{ "com.apple.nvidia.driver.NVDAGK100Hal", kextNVDAGK100Hal, 2, true, {}, KernelPatcher::KextInfo::Unloaded }
};

static size_t kextListSize {1};

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

void NVRESL::deinit() {
	
}

void NVRESL::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	for (size_t i =0; i < kextListSize; i++) {
		if (kextList[i].loadIndex == index) {
			if (!(progressState & ProcessingState::NVReslPatched) && strcmp(kextList[i].id, "com.apple.nvidia.driver.NVDAGK100Hal")) {
				DBGLOG("cdf @ NVPatcher found com.apple.nvidia.driver.NVDAGK100Hal");
				
				//
				// Reference:
				// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/NVIDIA-patcher.command
				//
				
				// patch #1-1 start
				const uint8_t find1_1[] = { 0xC7, 0x82, 0xC8, 0x00, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
				const uint8_t repl1_1[] = { 0xC7, 0x82, 0xC8, 0x00, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
			
				KextPatch kext_patch_1_1 {
					{ &kextList[i], find1_1, repl1_1, sizeof(find1_1), 1 },
					KernelVersion::MountainLion, KernelVersion::Sierra
				};
				applyPatches(patcher, index, &kext_patch_1_1, 1);
				progressState |= ProcessingState::NVReslPatched;
				DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 1-1");
				// patch #1-1 end
				
				// patch #1-2 start
				const uint8_t find1_2[] = { 0xC7, 0x82, 0x10, 0x01, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
				const uint8_t repl1_2[] = { 0xC7, 0x82, 0x10, 0x01, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
				
				KextPatch kext_patch_1_2 {
					{ &kextList[i], find1_2, repl1_2, sizeof(find1_2), 1 },
					KernelVersion::MountainLion, KernelVersion::Sierra
				};
				applyPatches(patcher, index, &kext_patch_1_2, 1);
				progressState |= ProcessingState::NVReslPatched;
				DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 1-2");
				// patch #1-2 end
				
				// patch #2-1 start
				const uint8_t find2_1[] = { 0xC7, 0x82, 0xD0, 0x00, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
				const uint8_t repl2_1[] = { 0xC7, 0x82, 0xD0, 0x00, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
				
				KextPatch kext_patch_2_1 {
					{ &kextList[i], find2_1, repl2_1, sizeof(find2_1), 1 },
					KernelVersion::MountainLion, KernelVersion::Sierra
				};
				applyPatches(patcher, index, &kext_patch_2_1, 1);
				progressState |= ProcessingState::NVReslPatched;
				DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 2-1");
				// patch #2-1 end
				
				// patch #2-2 start
				const uint8_t find2_2[] = { 0xC7, 0x82, 0x20, 0x01, 0x00, 0x00, 0x88, 0x84, 0x02, 0x00 };
				const uint8_t repl2_2[] = { 0xC7, 0x82, 0x20, 0x01, 0x00, 0x00, 0x80, 0x1A, 0x06, 0x00 };
				
				KextPatch kext_patch_2_2 {
					{ &kextList[i], find2_2, repl2_2, sizeof(find2_2), 1 },
					KernelVersion::MountainLion, KernelVersion::Sierra
				};
				applyPatches(patcher, index, &kext_patch_2_2, 1);
				progressState |= ProcessingState::NVReslPatched;
				DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 2-2");
				// patch #2-2 end
				
				// patch #3 start
				const uint8_t find3[] = { 0x8B, 0x82, 0xD0, 0x00, 0x00, 0x00, 0xB9, 0x88, 0x84, 0x02, 0x00 };
				const uint8_t repl3[] = { 0x8B, 0x82, 0xD0, 0x00, 0x00, 0x00, 0xB9, 0x80, 0x1A, 0x06, 0x00 };
				
				KextPatch kext_patch_3 {
					{ &kextList[i], find3, repl3, sizeof(find3), 1 },
					KernelVersion::MountainLion, KernelVersion::Sierra
				};
				applyPatches(patcher, index, &kext_patch_3, 1);
				progressState |= ProcessingState::NVReslPatched;
				DBGLOG("cdf @ NVPatcher patched NVDAGK100Hal version 3");
				// patch #3 end
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
