//
//  IntelPatcher.cpp
//  IntelPatcher (CoreDisplayFixup)
//
//  Copyright © 2017 Vanilla. All rights reserved.
//
//  This module is based on vit9696's Shiki.
//

#include "IntelPatcher.hpp"

// target frameworks
static const char *binaryList[] {
	// 10.10.x and 10.11.x
	"/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit",
	// as of 10.12.x
	"/System/Library/Frameworks/CoreDisplay.framework/Versions/A/CoreDisplay"
};

// accompanied processes
static const char *procList[] {
	// 10.10.x or 10.11.x
	"/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer",
	// as of 10.12.x
	"/System/Library/PrivateFrameworks/SkyLight.framework/Versions/A/Resources/WindowServer"
};

// Patches
//
// Reference:
// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/CoreDisplay-patcher.command
//
// Modified by PMheart (jmpq adress optimisations)
//
static const uint8_t findBufOld[] {
	0xB8, 0x01, 0x00, 0x00, 0x00,                   // mov  eax, 0x1
	0xF6, 0xC1, 0x01,                               // test cl, 0x1
	0x0F, 0x85                                      // jne  "somewhere"  ; Don't care for the exact offset!
};

static const uint8_t replBufOld[] {
	0x33, 0xC0,                                     // xor eax, eax      ; 0
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,       // nop (7x)          ; placeholders
	0xE9                                            // jmp "somewhere"   ; Don't care for the exact offset!
};

static const size_t bufSizeOld = arrsize(findBufOld);

// 10.13.4+
static const uint8_t findBuf1034[] {
	0xBB, 0x01, 0x00, 0x00, 0x00,                   // mov ebx, 0x1
	0xA8, 0x01,                                     // test al, 0x1
	0x0F, 0x85                                      // jne <somewhere>
};

static const uint8_t replBuf1034[] {
	0x31, 0xDB,                                     // xor ebx, ebx
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90,             // nop (6x)
	0xE9                                            // jmp <somewhere>
};

static const size_t bufSize1034 = arrsize(findBuf1034);

static UserPatcher::BinaryModPatch patchOld {
	CPU_TYPE_X86_64,
	findBufOld,
	replBufOld,
	bufSizeOld,
	0,            // skip  = 0 -> replace all occurrences
	1,            // count = 1 -> 1 set of hex inside the target binaries
	UserPatcher::FileSegment::SegmentTextText,
	SectionHS1034 // 10.10.x till 10.13.3 (all universal)
};

static UserPatcher::BinaryModPatch patch1034 {
	CPU_TYPE_X86_64,
	findBuf1034,
	replBuf1034,
	bufSize1034,
	0,            // skip  = 0 -> replace all occurrences
	1,            // count = 1 -> 1 set of hex inside the target binaries
	UserPatcher::FileSegment::SegmentTextText,
	SectionHS1034 // 10.13.4+
};

UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[] {
	// 10.10.x and 10.11.x
	{ binaryList[0], &patchOld, 1 }
};

UserPatcher::BinaryModInfo ADDPR(binaryModSieHS)[] {
	// 10.12.x and 10.13.0-10.13.3
	{ binaryList[1], &patchOld, 1 }
};

UserPatcher::BinaryModInfo ADDPR(binaryModHS1034)[] {
	// 10.13.4+
	{ binaryList[1], &patch1034, 1 }
};

const size_t ADDPR(binaryModOldSize) = 1;
const size_t ADDPR(binaryModHS1034Size) = 1;


UserPatcher::ProcInfo ADDPR(procInfoYosEC)[] {
	// 10.10.x and 10.11.x
	{ procList[0], static_cast<uint32_t>(strlen(procList[0])), SectionYosEC }
};

UserPatcher::ProcInfo ADDPR(procInfoSieHS)[] {
	// 10.12.x and 10.13.x
	{ procList[1], static_cast<uint32_t>(strlen(procList[1])), SectionSieHS }
};

const size_t ADDPR(procInfoSize) = 1;
