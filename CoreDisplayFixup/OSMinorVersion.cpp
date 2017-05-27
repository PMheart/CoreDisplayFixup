//
//  OSVersion.cpp
//  CoreDisplayFixup
//
//  Created by Vanilla on 5/28/17.
//  Copyright © 2017 Vanilla. All rights reserved.
//

#include "OSMinorVersion.hpp"

#include <libkern/version.h>

int OSVersion::getMinorVersion() {
	// Reference:
	// https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/CoreDisplay-patcher.command
	//
	// Apple has reversed Darwin 16.2.0 due to unknown reasons,
	// and thus we can't get the real macOS version,
	// so here we introduce a specific algo to do that.
	if (version_major == 16 && version_minor > 2)
		this->minor = version_minor - 1;
	else
		this->minor = version_minor;
	
	return this->minor;
}
