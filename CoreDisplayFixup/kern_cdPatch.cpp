//
//  kern_cdPatch.cpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

//
// CoreDisplayFixup::Headers
//
#include "kern_cdPatch.hpp"


//
// The patch(es) that to be applied.
//

alignas(8) static const UINT8 KBEYosECSieSearch[]
{
    0xB8, 0x01, 0x00, 0x00, 0x00,                   // mov eax, 0x1
    0xF6, 0xC1, 0x01,                               // test cl, 0x1
    0x0F, 0x85                                      // jne "somewhere" ; Don't care for the exact offset!
};

alignas(8) static const UINT8 KBEYosECSieReplace[]
{
    0x33, 0xC0,                                     // xor eax, eax    ; 0
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,       // 7x nop          ; placeholders
    0xE9                                            // jmp "somewhere" ; Don't care for the exact offset!
};


//============================================================================================================


//
// Structure holding lookup-style binary patches.
//

static UserPatcher::BinaryModPatch genericPatch[]
{
	{
        CPU_TYPE_X86_64,
        KBEYosECSieSearch,
        KBEYosECSieReplace,
        10,
        0,
        1,
        UserPatcher::FileSegment::SegmentTextText,
        SectionYosEC // 10.10.5 till 10.12.x
    }
};


//============================================================================================================


//
// Structure describing the modifications for the binary.
//


UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[]
{
    {
        "/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit",
        genericPatch,
        1
    }
};

UserPatcher::BinaryModInfo ADDPR(binaryModSie)[]
{
    {
        "/System/Library/Frameworks/CoreDisplay.framework/Versions/A/CoreDisplay",
        genericPatch,
        1
    }
};


//============================================================================================================


//
// The amount of provided binary modifications.
//
const size_t ADDPR(binaryModSize) {1};


//
// Process(es) declarations.
//

UserPatcher::ProcInfo ADDPR(procInfoYosEC)[]
{
    {
        "/System/Library/Frameworks/CoreGraphics.framework/Versions/A/Resources/WindowServer",
        83,
        SectionYosEC
    }
};

UserPatcher::ProcInfo ADDPR(procInfoSie)[]
{
    {
        "/System/Library/PrivateFrameworks/SkyLight.framework/Versions/A/Resources/WindowServer",
        86,
        SectionSie
    }
};


//============================================================================================================


//
// The amount of provided processes.
//
const size_t ADDPR(procInfoSize) {1};
