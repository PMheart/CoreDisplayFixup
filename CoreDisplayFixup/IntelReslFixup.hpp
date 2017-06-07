//
//  IntelReslFixup.hpp
//  CoreDisplayFixup
//
//  Copyright © 2017 vit9696, Vanilla. All rights reserved.
//
//  This kext is made based on vit9696's Shiki, without his amazing repo it won't be here!
//

#include <Headers/kern_user.hpp>

// binary
extern UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[];
extern UserPatcher::BinaryModInfo ADDPR(binaryModSie)[];
extern const size_t ADDPR(binaryModSize);

// process
extern UserPatcher::ProcInfo ADDPR(procInfoYosEC)[];
extern UserPatcher::ProcInfo ADDPR(procInfoSieHigh)[];
extern const size_t ADDPR(procInfoSize);


enum : uint32_t {
  SectionYosEC = 1,
  SectionSie   = 2
};
