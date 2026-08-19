# Install and first run

## Current supported public-development path

The current reproducible path is a debug build installed with ADB. A one-click consumer APK with a graphical game-data importer is not published yet.

### Requirements

- Android device with ARM64 support
- USB debugging enabled
- Android platform-tools (`adb`) available on the host
- A debug APK built from this source snapshot
- Your own compatible System Shock `res` directory

### Install + copy data + launch

```powershell
pwsh -File .\scripts\install-debug.ps1 -GameRes 'D:\Path\To\res'
```

If you have multiple ADB devices connected, disconnect the others before using the current helper.

## Reference device

The validated hardware reference is the Retroid Pocket 5. The code is Android/ARM64, but that does not imply every Android device/controller/display combination has been validated. See `COMPATIBILITY.md`.
