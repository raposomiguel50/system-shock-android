# Install and first run

## Current QA path

The preferred hardware-test path is the dedicated QA build. It uses the package ID `com.rp5np.systemshock.qa`, so it can coexist with an existing `com.rp5np.systemshock` installation on the same device.

A one-click consumer APK with a graphical game-data importer is not published yet.

### Requirements

- Android device with ARM64 support
- USB debugging enabled
- Android platform-tools available in the pinned Android SDK
- A QA APK built and verified from this source tree
- Your own compatible System Shock `res` directory containing `data` and `sound`

### Build and verify QA

```powershell
pwsh -File .\scripts\qa-gate.ps1
```

### Install QA + copy data + launch

```powershell
pwsh -File .\scripts\install-qa.ps1 -GameRes 'D:\Path\To\res'
```

The QA installer verifies the APK before installation, targets only `com.rp5np.systemshock.qa`, and compares the normal `com.rp5np.systemshock` package path before and after installation. If the baseline path changes, the helper fails instead of continuing.

If multiple ADB devices are connected, disconnect the devices that are not part of the test.

## Debug path

`scripts/install-debug.ps1` remains available for developer workflows that intentionally target `com.rp5np.systemshock`. It is not the preferred path when a functional baseline installation must be preserved.

## Reference device

The Retroid Pocket 5 is the primary hardware reference. The code is Android/ARM64, but that does not imply every Android device/controller/display combination has been validated. See `COMPATIBILITY.md`.
