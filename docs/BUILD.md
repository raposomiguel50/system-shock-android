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

The build scripts run `scripts/preflight.ps1` automatically. It verifies JDK 17, the pinned Android toolchain, exact dependency commits, a clean tracked Git working tree, and release-signing configuration when required.

## 3. Build variants

### Debug

```powershell
pwsh -File .\scripts\build.ps1 -Variant debug
```

Package: `io.github.raposomiguel50.systemshock`

### QA

```powershell
pwsh -File .\scripts\build.ps1 -Variant qa
```

Package: `io.github.raposomiguel50.systemshock.qa`

The QA build uses the generic debug key and can coexist with both the stable package and the historical pre-release package `com.rp5np.systemshock`.

### Release

The release variant is non-debuggable and requires explicit release-signing configuration. See `docs/RELEASE.md`.

```powershell
pwsh -File .\scripts\build.ps1 -Variant release
```

## 4. Verify the APK

```powershell
pwsh -File .\scripts\verify-apk.ps1 -Variant qa
```

The verifier checks expected package ID, version, label, SDK levels, ARM64-only native code, required native libraries, obvious proprietary-data absence, debuggable state and APK signature semantics.

For `release`/`releaseQa`, `RP5NP_RELEASE_CERT_SHA256` is mandatory and pins the exact stable signing identity expected for that build.

## 5. Side-by-side hardware QA

When a test device is connected and authorized through ADB, the QA installer can load independently obtained game data into the QA package:

```powershell
pwsh -File .\scripts\install-qa.ps1 -GameRes 'C:\path\to\owned\res'
```

The helper verifies the QA APK and checks that neither the stable package nor the historical pre-release package is changed by QA installation.

## Fresh-clone reproducibility gate

A release candidate must pass:

1. PowerShell static validation.
2. Dependency bootstrap.
3. Preflight.
4. QA build.
5. APK verification.
6. Side-by-side hardware validation when hardware testing is required.
7. Signed release build from the accepted source commit.
8. Release APK verification and hash recording.

The `.cxx` and Gradle build directories are path-bound disposable build state and should not be treated as portable.
