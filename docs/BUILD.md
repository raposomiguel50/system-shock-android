# Build from source - Windows / PowerShell 7

## Scope

This is the reproducible Android/ARM64 build path for the public source tree. It is designed around Windows 11 and PowerShell 7 and does not depend on the private development workspace.

## Required tools

- Git
- PowerShell 7
- JDK 17
- Android SDK Platform 34
- Android Build Tools 34.0.0
- Android NDK `29.0.14206865`
- CMake 3.22.1 from the Android SDK
- Android platform-tools only when installing or testing on hardware

The repository includes the Gradle wrapper and uses Gradle 8.1.1 with Android Gradle Plugin 8.1.1.

## 1. Bootstrap pinned public dependencies

From the repository root:

```powershell
pwsh -File .\scripts\bootstrap-deps.ps1
```

This creates the ignored `.deps/` directory and checks out:

- SDL 2.32.10 at `5d249570393f7a37e037abf22cd6012a4cc56a71`
- SDL_mixer 2.8.1 at `171eb2d420d5643e4ee11514a06e04a41a463bbd`

## 2. Preflight

The build scripts run `scripts/preflight.ps1` automatically. It verifies:

- JDK 17
- the pinned Android SDK, Build Tools, NDK and CMake versions
- the exact dependency commits
- a clean tracked Git working tree
- release signing configuration when the release variant is requested

It can also be run directly:

```powershell
pwsh -File .\scripts\preflight.ps1 -Variant qa
```

## 3. Build variants

### Debug

For normal developer work:

```powershell
pwsh -File .\scripts\build.ps1 -Variant debug
```

Package: `com.rp5np.systemshock`

### QA

For real-device testing without touching an existing baseline installation:

```powershell
pwsh -File .\scripts\build.ps1 -Variant qa
```

Package: `com.rp5np.systemshock.qa`

The QA build type is initialized from the debug build type, uses the generic debug signing key and has its own application ID. It can coexist with `com.rp5np.systemshock` on the same device.

### Release

The release variant is non-debuggable and requires an explicit release signing configuration. See `docs/RELEASE.md`.

```powershell
pwsh -File .\scripts\build.ps1 -Variant release
```

The build refuses to enter the release path unless the required release-signing environment variables are present and the keystore exists.

## 4. Verify the APK

Always verify the exact variant that was built:

```powershell
pwsh -File .\scripts\verify-apk.ps1 -Variant qa
```

The verifier checks:

- expected package ID
- version code and version name
- minimum and target SDK
- ARM64-only native code
- required native libraries
- absence of obvious commercial System Shock data
- compiled launcher icon
- expected debuggable state
- APK signature validity
- rejection of the generic Android debug certificate for release builds

## 5. Side-by-side hardware QA

When a Retroid Pocket 5 or another Android test device is connected and authorized through ADB, the QA installer can load independently obtained game data into the QA package:

```powershell
pwsh -File .\scripts\install-qa.ps1 -GameRes 'C:\path\to\owned\res'
```

The helper verifies the QA APK before installation, targets only `com.rp5np.systemshock.qa`, and checks that the normal `com.rp5np.systemshock` package path is unchanged before and after QA installation.

## Fresh-clone reproducibility gate

A release candidate must pass the following sequence from a new clone created directly at its final test path:

1. PowerShell static validation.
2. Dependency bootstrap.
3. Preflight.
4. QA build.
5. APK verification.
6. Side-by-side installation on real hardware.
7. Import of independently obtained game data into the QA package.
8. Runtime validation of display, controls, text input, audio and gameplay.
9. Signed release build from the accepted source commit.
10. Release APK verification and hash recording.

The `.cxx` and Gradle build directories are treated as path-bound disposable build state. A configured working tree must not be moved and reused as if those caches were portable.
