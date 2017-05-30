CoreDisplayFixup Changelog
============================
#### v1.2.2
- Added ``Maxwell`` and ``Pascal`` support for NVPatcher
- Simplified the procedure for NVPatcher, thanks [vit9696](https://github.com/vit9696)
- Added ``OSBundleCompatibleVersion`` for future use

#### v1.2.1
- Kernel flags `-cdfinteloff` and `-cdfnvoff` are now available to disable particular patcher(s).
  - `-cdfinteloff` to disable IntelPatcher
  - `-cdfnvoff` to disable NVPatcher
  - `-cdfoff` to disable CoreDisplayFixup entirely (Similar to boot with `-cdfinteloff` and `-cdfnvoff` simultaneously)
- Removed redundant headers
- Changed all tab sizes to 2
- Changed compiler optimization level to reduce kext size
- Minor repository structure changes


#### Older commits
They can be tracked at [here](https://github.com/PMheart/CoreDisplayFixup/commits).
