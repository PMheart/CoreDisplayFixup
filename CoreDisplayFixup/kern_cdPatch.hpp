//
//  kern_cdPatch.hpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

//
// Lilu::Headers
//
#include <Headers/kern_user.hpp>


//
// Type definitions, for convenience.
//
typedef uint8_t  UINT8;
typedef uint32_t UINT32;


//
// Declaration of patch(es) for 10.10 (Darwin 14).
//
extern UserPatcher::BinaryModInfo ADDPR(binaryModYosemite)[];
//
// Declaration of patch(es) for 10.11 (Darwin 15).
//
extern UserPatcher::BinaryModInfo ADDPR(binaryModCapitan)[];
//
// Declaration of patch(es) for 10.12 (Darwin 16).
//
extern UserPatcher::BinaryModInfo ADDPR(binaryModSierra)[];
//
// The amount of provided binary modifications.
//
extern const size_t ADDPR(binaryModSize);


//
// Process(es) of patch(es) for 10.10 (Darwin 14).
//
extern UserPatcher::ProcInfo ADDPR(procInfoYosemite)[];
//
// Process(es) of patch(es) for 10.11 (Darwin 15).
//
extern UserPatcher::ProcInfo ADDPR(procInfoCapitan)[];
//
// Process(es) of patch(es) for 10.12 (Darwin 16).
//
extern UserPatcher::ProcInfo ADDPR(procInfoSierra)[];
//
// The amount of provided processes.
//
extern const size_t ADDPR(procInfoSize);


//
// Section declarations.
//
enum : UINT32
{
    SectionYosemite   = 1,
    SectionElCapitan  = 2,
    SectionSierra     = 3
};
