//
//  kern_cdPatch.hpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
// This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include <Headers/kern_user.hpp>

typedef uint8_t UINT8;
typedef uint32_t UINT32;

//
// How to patch the binaries?
//
extern UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[];  // 10.10.x and 10.11.x
extern UserPatcher::BinaryModInfo ADDPR(binaryModSie)[];    // 10.12.x
extern size_t ADDPR(binaryModSize);

//
// What are the related process(es)?
//
extern UserPatcher::ProcInfo ADDPR(procInfoYosEC)[];        // 10.10.x and 10.11.x
extern UserPatcher::ProcInfo ADDPR(procInfoSie)[];          // 10.12.x
extern size_t ADDPR(procInfoSize);


// Section list start

enum : UINT32 {
    SectionUnused = 0,  // 0 means aborting patch
    SectionYosEC  = 1,
    SectionSie    = 2
};

// Section list end


// EOF
