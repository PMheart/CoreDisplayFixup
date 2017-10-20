CoreDisplayFixup Installation & Usage
======================================

#### System Requirements
CoreDisplayFixup needs macOS ***v10.10*** or greater.

#### Installation
It's highly recommended to let the bootloader inject CoreDisplayFixup, otherwise you'll need [LiluFriend](https://github.com/PMheart/LiluFriend) to ensure CoreDisplayFixup will work properly.

#### Available kernel flags
Add `-cdfdbg` to enable debug logging (ONLY available in DEBUG binaries).

Add `-cdfoff` to disable CoreDisplayFixup entirely.

Add `-cdfinteloff` to disable IntelPatcher, which cancels patching `IOKit` or `CoreDisplay`.

Add `-cdfnvoff` to disable NVPatcher that cancels patching NVidia related kexts.

Add `-cdfbeta` to enable CoreDisplayFixup on unsupported OS versions (Not compatible with IntelPatcher).
