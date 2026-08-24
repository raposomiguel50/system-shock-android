# Continuous integration boundary

## Purpose

GitHub Actions validates the redistributable source and unsigned/debug-signable QA path without exposing the private stable release-signing key.

The Android QA workflow runs on Windows and performs:

1. source checkout;
2. JDK 17 setup;
3. installation/verification of the pinned Android platform, Build Tools, NDK and CMake versions;
4. pinned PSScriptAnalyzer installation;
5. PowerShell parse/static validation;
6. bootstrap of the pinned public SDL and SDL_mixer dependencies;
7. isolated QA APK build;
8. semantic APK verification;
9. short-lived QA artifact upload for inspection.

The workflow uses Node.js 24-compatible generations of the official checkout, setup-java and upload-artifact actions.

## What CI proves

A successful run proves that the tested public source can pass the scripted QA build and verifier in a fresh hosted environment using the documented public toolchain/dependencies.

The verifier checks the expected v1 package/version/label/ABI, required native libraries, obvious proprietary-data exclusion, debuggable state and APK signature semantics appropriate to the QA variant.

## What CI deliberately does not prove

The stable private release key is not stored in the repository or required by the public QA workflow. Therefore CI does not produce the official signed `com.rp5np.systemshock` release APK.

The final stable signing/build proof is performed locally from a clean checkout with the private release-signing environment by:

```powershell
pwsh -File .\scripts\v1-final-gate.ps1
```

Real-hardware/manual acceptance is also a separate evidence class. The v1 preservation scope records the accepted complete playthrough on the Retroid Pocket 5 reference target.

## Release evidence

The exact successful GitHub Actions run for the final source commit is referenced in release evidence rather than hard-coded into this source file. This avoids changing the source commit merely to record a run that validates that same commit.

See [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md), [`RELEASE.md`](RELEASE.md) and [`INTEGRITY.md`](INTEGRITY.md).
