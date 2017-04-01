CoreDisplayFixup
===================

An open source kernel extension which targets at patching IOKit/CoreDisplay dynamically to enable 4K or greater resolutions output on non-Iris Intel Graphics.

#### Notes
This repository should be compiled with [Lilu](https://github.com/vit9696/Lilu) v1.1.0 and greater, for otherwise the compilation will be failed! (*** Debug version of Lilu.kext should be put in the same folder as CoreDisplayFixup! And they should be also used together! ***)

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu) and some ideas
- [Floris497](https://github.com/Floris497) for the [patches](https://github.com/Floris497/mac-pixel-clock-patch-v2)
- [gujiangjiang](https://github.com/gujiangjiang) for major tests
- [PMheart](https://github.com/PMheart) for writing the software and maintaining it