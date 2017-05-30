CoreDisplayFixup Installation & Usage
======================================

#### Installation
It's highly recommended to let the bootloader inject CoreDisplayFixup, otherwise it may rarely fail.

#### Available kernel flags
Add `-cdfdbg` to enable debug logging (ONLY available in DEBUG binaries).
Add `-cdfoff` to disable CoreDisplayFixup entirely.
Add `-cdfinteloff` to disable IntelPatcher, which cancels patching `IOKit` or `CoreDisplay`.
Add `-cdfnvoff` to disable NVPatcher that cancels patching `NVDAGK100Hal`.
Add `-cdfbeta` to enable CoreDisplayFixup on unsupported OS versions.
