//
//    kern_cdPatch.cpp
//    CoreDisplayFixup
//
//    Created by Vanilla on 3/26/17.
//    Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//
//    This kext is made based on vit9696's Shiki, without his amazing repo the repo won't be here!
//

#include "kern_cdPatch.hpp"

// bin-patch section start

const uint8_t searchBuf[] { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xF6, 0xC1, 0x01, 0x0F, 0x85 };  // mov 0x1, eax | test 0x1, cl | jne 0x15189
const uint8_t replaceBuf[] { 0x33, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9 }; // xor eax, eax | 7x nop | jmp 0x15189

// bin-patch section end


// Patch section start

UserPatcher::BinaryModPatch genericPatch[] {
	{ CPU_TYPE_X86_64, searchBuf, replaceBuf, 10, 0, 1, UserPatcher::FileSegment::SegmentTextText, SectionYosemite },   // 10.10.5 till 10.12.x
};

// Patch section end


// Mod section start

UserPatcher::BinaryModInfo ADDPR(binaryModYosemite)[] {
    { "/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit", genericPatch, 1 },
};

UserPatcher::BinaryModInfo ADDPR(binaryModCapitan)[] {
    { "/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit", genericPatch, 1 },
};

UserPatcher::BinaryModInfo ADDPR(binaryModSierra)[] {
    { "/System/Library/Frameworks/CoreDisplay.framework/Versions/A/CoreDisplay", genericPatch, 1 },
};


const size_t ADDPR(binaryModSize) {1};

// Mod section end


// Process list start

UserPatcher::ProcInfo ADDPR(procInfoYosemite)[] {
    { "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer", 83, SectionYosemite }
};

UserPatcher::ProcInfo ADDPR(procInfoCapitan)[] {
    { "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer", 83, SectionCapitan }
};

UserPatcher::ProcInfo ADDPR(procInfoSierra)[] {
    { "/System/Library/PrivateFrameworks/SkyLight.framework/Versions/A/Resources/WindowServer", 86, SectionSierra }
};

const size_t ADDPR(procInfoSize) {1};

// Process list end


// EOF
