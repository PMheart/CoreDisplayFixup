CoreDisplayFixup
===================

A [Lilu](https://github.com/vit9696/Lilu) plug-in kernel extension which targets at patching IOKit/CoreDisplay dynamically to enable 4K or greater resolutions output on non-Iris Intel Graphics.

#### Notes
This repository should be compiled with [Lilu](https://github.com/vit9696/Lilu) v1.1.0 and greater, for otherwise the compilation will be failed! 

Note: ***Debug version of Lilu.kext should be put in the same folder as CoreDisplayFixup! And they should be also used together!***

Well, addtionally you may have a corrupted shared cache, to fix it up, disable CoreDisplayFixup and reboot, then run ```sudo update_dyld_shared_cache -force``` command in Terminal.

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu) and some ideas
- [Floris497](https://github.com/Floris497) for the [patches](https://github.com/Floris497/mac-pixel-clock-patch-v2)
- [gujiangjiang](https://github.com/gujiangjiang) and [wmchris](https://github.com/wmchris) for major tests
- [PMheart](https://github.com/PMheart) for writing the software and maintaining it

#### Bug reports
All bugs should be reported at [the issue section of this repository](https://github.com/PMheart/CoreDisplayFixup/issues).
