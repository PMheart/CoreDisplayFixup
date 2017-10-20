CoreDisplayFixup
===================

A [Lilu](https://github.com/vit9696/Lilu) plug-in which patches system essentials dynamically to enable 4K or greater resolutions output on certain graphics (See [here](https://github.com/Floris497/mac-pixel-clock-patch-V2/blob/master/README.md) for more details).

#### Notes
This repository should be compiled with [Lilu](https://github.com/vit9696/Lilu) ***v1.2.0*** and greater, otherwise the compilation will be failed!

Note: ***Debug version of Lilu.kext should be put in the same folder as CoreDisplayFixup! And they should also be used together!***

Addtionally you may have a corrupted shared cache, to fix it up, disable CoreDisplayFixup and reboot, then run `sudo update_dyld_shared_cache -force` command in Terminal.

#### Configuration
See [Instructions](https://github.com/PMheart/CoreDisplayFixup/blob/master/Instructions.md) for more details.

#### Support and discussion
- [InsanelyMac topic](http://www.insanelymac.com/forum/topic/322841-coredisplayfixup-fixing-certain-problems-for-4k-or-greater-resolution-on-non-iris-intel-graphics/) in English
- [PCBeta aka 远景论坛](http://bbs.pcbeta.com/viewthread-1736985-1-1.html) in Chinese/中文
- [GitHub Issues](https://github.com/PMheart/CoreDisplayFixup/issues)

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu) and some ideas
- [Floris497](https://github.com/Floris497) for the [patches](https://github.com/Floris497/mac-pixel-clock-patch-v2)
- [gujiangjiang](https://github.com/gujiangjiang) and [wmchris](https://github.com/wmchris) for major tests
- [PMheart](https://github.com/PMheart) for writing the software and maintaining it

#### Bug reports
All bugs should be reported at [the issue section of this repository](https://github.com/PMheart/CoreDisplayFixup/issues).
