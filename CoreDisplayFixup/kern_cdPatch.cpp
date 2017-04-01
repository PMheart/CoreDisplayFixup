//
//  kern_cdPatch.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
// This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include "kern_cdPatch.hpp"

// bin-patch section start

// for all supported versions (10.10.x, 10.11.x, 10.12.x)
// the patch is from https://github.com/Floris497/mac-pixel-clock-patch-v2 , thanks to Floris497!

//
// mov 0x1, eax | test 0x1, cl | jne 0x15189 (don't care for the offset!)
//
UINT8 displayKBEYosECSieSearch[]  = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xF6, 0xC1, 0x01, 0x0F, 0x85 };

//
// xor eax, eax | 7x nop       | jmp 0x15189 (don't care for the offset!)
//
UINT8 displayKBEYosECSieReplace[] = { 0x33, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9 };

// bin-patch section end


// Patch section start

UserPatcher::BinaryModPatch genericPatch[] {
	{ CPU_TYPE_X86_64, displayKBEYosECSieSearch, displayKBEYosECSieReplace, 10, 0, 1, UserPatcher::FileSegment::SegmentTextText, SectionYosEC }   // 10.10.5 till 10.12.x
};

// Patch section end


// Mod section start

UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[] {
	{ "/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit", genericPatch, 1 }
};

UserPatcher::BinaryModInfo ADDPR(binaryModSie)[] {
	 { "/System/Library/Frameworks/CoreDisplay.framework/Versions/A/CoreDisplay", genericPatch, 1 }
};


size_t ADDPR(binaryModSize) {1};

// Mod section end


// Process list start

UserPatcher::ProcInfo ADDPR(procInfoYosEC)[] {
	{ "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer", 83, SectionYosEC }
};

UserPatcher::ProcInfo ADDPR(procInfoSie)[] {
	{ "/System/Library/PrivateFrameworks/SkyLight.framework/Versions/A/Resources/WindowServer", 86, SectionSie }
};

size_t ADDPR(procInfoSize) {1};

// Process list end


// EOF
