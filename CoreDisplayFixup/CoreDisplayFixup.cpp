//
//  kern_cdPatch.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include "CoreDisplayFixup.hpp"

static const size_t bufSize = 10;

// Reference:
// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/CoreDisplay-patcher.command
static const uint8_t findBuf[bufSize] {
    0xB8, 0x01, 0x00, 0x00, 0x00,                   // mov eax, 0x1
    0xF6, 0xC1, 0x01,                               // test cl, 0x1
    0x0F, 0x85                                      // jne "somewhere" ; Don't care for the exact offset!
};

static const uint8_t replBuf[bufSize] {
    0x33, 0xC0,                                     // xor eax, eax    ; 0
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,       // 7x nop          ; placeholders
    0xE9                                            // jmp "somewhere" ; Don't care for the exact offset!
};


static UserPatcher::BinaryModPatch genericPatch {
        CPU_TYPE_X86_64,
        findBuf,
        replBuf,
        bufSize,
        0,			 // skip
        1,			 // count
        UserPatcher::FileSegment::SegmentTextText,
        SectionYosEC // 10.10.x till 10.12.x
};


static const size_t binSize = 2;

static const char *binaryList[binSize] {
	"/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit",				// 10.10.x and 10.11.x
	"/System/Library/Frameworks/CoreDisplay.framework/Versions/A/CoreDisplay",	// as of 10.12.x
};

UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[] { // 10.10.x and 10.11.x
	binaryList[0], &genericPatch, 1
};

UserPatcher::BinaryModInfo ADDPR(binaryModSie)[] { // as of 10.12.x
	binaryList[1], &genericPatch, 1
};

const size_t ADDPR(binaryModSize) {1};


static const size_t procSize = 2;
static const char *procList[procSize] {
	"/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer",		// 10.10.x and 10.11.x
	"/System/Library/PrivateFrameworks/SkyLight.framework/Versions/A/Resources/WindowServer",	// as of 10.12.x
};

UserPatcher::ProcInfo ADDPR(procInfoYosEC)[] {
	procList[0], 83, SectionYosEC
};

UserPatcher::ProcInfo ADDPR(procInfoSie)[] {
	procList[1], 86, SectionSie
};

const size_t ADDPR(procInfoSize) {1};
