//
//    kern_cdPatch.hpp
//    CoreDisplayFixup
//
//    Created by Vanilla on 3/26/17.
//    Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//    This kext is made based on vit9696's Shiki, without his amazing repo the repo won't be here!
//

#include <Headers/kern_user.hpp>

extern UserPatcher::BinaryModInfo ADDPR(binaryModYosemite)[];
extern UserPatcher::BinaryModInfo ADDPR(binaryModCapitan)[];
extern UserPatcher::BinaryModInfo ADDPR(binaryModSierra)[];
extern const size_t ADDPR(binaryModSize);


extern UserPatcher::ProcInfo ADDPR(procInfoYosemite)[];
extern UserPatcher::ProcInfo ADDPR(procInfoCapitan)[];
extern UserPatcher::ProcInfo ADDPR(procInfoSierra)[];
extern const size_t ADDPR(procInfoSize);


// Section list

enum : uint32_t {
    SectionUnused = 0,
    SectionYosemite = 1,
    SectionCapitan = 2,
    SectionSierra = 3
};
