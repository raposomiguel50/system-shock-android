# Build from source - Windows / PowerShell 7

## Scope

This is the reproducible developer build path for the current Android/ARM64 snapshot. It was designed around Windows 11 and PowerShell 7. It does **not** require the original private development workspace.

## Required tools

Install these before building:

- Git
- PowerShell 7
- JDK 17
- Android SDK Platform 34
- Android Build Tools 34.0.0
- Android NDK `29.0.14206865`
- CMake 3.22.1 from the Android SDK
- Android platform-tools (`adb` is only required for device installation/testing)

The Gradle wrapper is included and uses Gradle 8.1.1.

## 1. Bootstrap public dependencies

From the repository root:

```powershell
pwsh -File .\scripts\bootstrap-deps.ps1
```

This creates `.deps/` and obtains:

- SDL 2.32.10 at commit `5d249570393f7a37e037abf22cd6012a4cc56a71`
- SDL_mixer 2.8.1 from the official `release-2.8.1` tag

`.deps/` is intentionally ignored by Git.

## 2. Build

If your Android SDK is in the normal Windows location, the build script will try `%LOCALAPPDATA%\Android\Sdk`. Otherwise pass it explicitly:

```powershell
pwsh -File .\scripts\build.ps1 -AndroidSdk 'D:\Android\Sdk'
```

The script verifies the required SDK/NDK/CMake directories, exports the two source dependency paths expected by `AndroidProject/app/jni/CMakeLists.txt`, and runs:

```text
AndroidProject\gradlew.bat --no-daemon :app:assembleDebug
```

Expected APK:

```text
AndroidProject\app\build\outputs\apk\debug\app-debug.apk
```

## 3. Verify the APK

```powershell
pwsh -File .\scripts\verify-apk.ps1
```

The verifier checks that the package contains the expected ARM64 native libraries and rejects obvious proprietary System Shock resource files inside the APK.

## Fresh-clone reproducibility gate

A release should only be called reproducible after the following succeeds from a clean clone/path with no dependency on the private development workspace:

1. Bootstrap dependencies.
2. Build the APK.
3. Verify the APK.
4. Install on a target device.
5. Supply independently obtained game data.
6. Reach the game and validate controls/audio/display.

Until this exact clean-clone sequence has been independently repeated, the project should be described as **reproducible-source candidate**, not universally reproduced on arbitrary Android hardware.
