//
//  IntelPatcher.hpp
//  IntelPatcher (CoreDisplayFixup)
//
//  Copyright © 2017 Vanilla. All rights reserved.
//
//  This module is based on vit9696's Shiki.
//

#ifndef IntelPatcher_hpp
#define IntelPatcher_hpp

#include <Headers/kern_user.hpp>

// binary
extern UserPatcher::BinaryModInfo ADDPR(binaryModYosEC)[];
extern UserPatcher::BinaryModInfo ADDPR(binaryModSieHS)[];
extern UserPatcher::BinaryModInfo ADDPR(binaryModHS10132)[];
extern const size_t ADDPR(binaryModSize);

// process
extern UserPatcher::ProcInfo ADDPR(procInfoYosEC)[];
extern UserPatcher::ProcInfo ADDPR(procInfoSieHS)[];
// assuming that the processes don't change until someone confirms it
// extern UserPatcher::ProcInfo ADDPR(procInfoHS10132)[];
extern const size_t ADDPR(procInfoSize);


enum : uint32_t {
	SectionYosEC   = 1,
	SectionSieHS   = 2,
	SectionHS10132 = 4
};

#endif /* IntelPatcher_hpp */
