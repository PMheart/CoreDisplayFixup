CoreDisplayFixup Changelog
============================
#### v1.3.5
Imported new patches of NVPatcher from aerror2's fork

#### v1.3.4
- Revert back to old IntelPatcher

#### v1.3.3
- Deprecated Version

#### v1.3.2
- NVPatcher 10.13 compatibility
- Code style changed

#### v1.3.1
- Various typo fixed

#### v1.3.0
- Requires Lilu 1.2.0 or higher

#### v1.2.8
- Fix for macOS v10.13

#### v1.2.7
- Clear error every time just in case

#### v1.2.6
- Added Lilu 1.1.8 safe mode loading compatibility

#### v1.2.5
- Enlarged compatibility

#### v1.2.4
- Added macOS High Sierra compatibility

#### v1.2.3
- Use getKernelMinorVersion() from Lilu to gather kernel minor version
- Changed some logics of NVPatcher to improve reliability
- Fixed kextListSize

#### v1.2.2
- Added ``Maxwell`` and ``Pascal`` support for NVPatcher
- Simplified the procedure for NVPatcher, thanks [vit9696](https://github.com/vit9696)
- Added ``OSBundleCompatibleVersion``

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
