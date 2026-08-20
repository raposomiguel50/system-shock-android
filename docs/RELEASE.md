# Release process

## Purpose

The release path is deliberately separate from normal development and QA. A public APK must not be produced from an arbitrary dirty workspace, a moved `.cxx` cache, or the generic Android debug signing key.

## Build identities

The Android project defines three build types:

- `debug`: normal local developer build using `com.rp5np.systemshock` and the generic debug key.
- `qa`: side-by-side hardware test build using `com.rp5np.systemshock.qa` and the generic debug key.
- `release`: public distribution build using `com.rp5np.systemshock`, non-debuggable, and a dedicated stable release key.

The current release metadata is:

- versionCode: `13`
- versionName: `0.1.0-pre.3`
- ABI: `arm64-v8a`

## Release signing boundary

The keystore and credentials are private material and must never be committed to this repository.

The release scripts read these environment variables:

- `RP5NP_RELEASE_STORE_FILE`
- `RP5NP_RELEASE_STORE_PASSWORD`
- `RP5NP_RELEASE_KEY_ALIAS`
- `RP5NP_RELEASE_KEY_PASSWORD`

An optional fifth variable can pin the expected certificate digest during verification:

- `RP5NP_RELEASE_CERT_SHA256`

`RP5NP_RELEASE_STORE_FILE` must point to the dedicated release keystore. The remaining variables contain the credentials required to access the signing key.

The same release key must be preserved for future versions. Losing it prevents compatible signed upgrades to users who installed earlier releases signed with that key.

## Release gate

From a clean checkout of the accepted source commit:

```powershell
pwsh -File .\scripts\bootstrap-deps.ps1
pwsh -File .\scripts\release-gate.ps1
```

`release-gate.ps1` performs:

1. PowerShell syntax/static checks.
2. Release preflight.
3. Exact release build.
4. APK semantic and signature verification.
5. Artifact copy to the ignored `dist/` directory.
6. SHA-256 generation.
7. A JSON manifest recording release version, source commit, package, ABI and APK hash.

Expected outputs:

```text
dist/SystemShock-Android-v0.1.0-pre.3-arm64-v8a.apk
dist/SystemShock-Android-v0.1.0-pre.3-arm64-v8a.apk.sha256
dist/SystemShock-Android-v0.1.0-pre.3-release.json
```

## Required gates before publishing the APK

A release is publishable only after all of these are true:

1. GitHub Actions QA workflow passes from the public source tree.
2. A fresh local clone passes bootstrap, QA build and APK verification.
3. The QA package is installed side-by-side on the Retroid Pocket 5 without changing the existing baseline package.
4. Independently obtained commercial game data is loaded only into the QA package.
5. Runtime QA passes for 1024x768 4:3 presentation, controls, fine cursor mode, Android text input, audio and representative gameplay.
6. The accepted commit is fixed and recorded.
7. The signed release build is generated from that accepted commit.
8. `verify-apk.ps1 -Variant release` passes.
9. The release APK SHA-256 and signing certificate SHA-256 are recorded in release evidence.
10. The Git tag and GitHub release point to the same accepted source commit.

## Data boundary

Commercial System Shock data is never embedded in the APK, Git repository, source archives, release artifacts or CI artifacts. Users must provide compatible data from a legally obtained copy separately.
