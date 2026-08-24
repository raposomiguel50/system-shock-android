# Release process

## Purpose

The release path is deliberately separate from normal development and QA. A public APK must not be produced from an arbitrary dirty workspace, a moved `.cxx` cache or the generic Android debug signing key.

Stable v1.0 is a preservation release. Its product boundary is defined in [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md), its integrity model in [`INTEGRITY.md`](INTEGRITY.md), and its final checklist in [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md).

## Build identities

The Android project defines four build types:

- `debug`: normal local developer build using `com.rp5np.systemshock` and the generic debug key;
- `qa`: side-by-side developer/hardware test build using `com.rp5np.systemshock.qa` and the generic debug key;
- `releaseQa`: side-by-side non-debuggable build using `com.rp5np.systemshock.releaseqa` and the stable release signing identity;
- `release`: public distribution build using `com.rp5np.systemshock`, non-debuggable, and the stable release key.

The v1.0.0 source metadata is:

- versionCode: `10000`
- versionName: `1.0.0`
- application label: `System Shock - Android`
- ABI: `arm64-v8a`
- package: `com.rp5np.systemshock`

The jump from pre-release `versionCode 13` to `10000` is intentional. It preserves Android's monotonic upgrade rule while establishing a simple stable-version numbering base.

## Release signing boundary

The keystore and credentials are private material and must never be committed to this repository.

The release scripts read these environment variables:

- `RP5NP_RELEASE_STORE_FILE`
- `RP5NP_RELEASE_STORE_PASSWORD`
- `RP5NP_RELEASE_KEY_ALIAS`
- `RP5NP_RELEASE_KEY_PASSWORD`

An optional fifth variable can independently pin the expected certificate digest during verification:

- `RP5NP_RELEASE_CERT_SHA256`

`RP5NP_RELEASE_STORE_FILE` must point to the dedicated release keystore. The remaining variables contain the credentials required to access the signing key.

The project verifier also hard-pins the established public signing certificate SHA-256:

`7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`

A release or release-QA APK signed with a different certificate fails verification. This prevents accidental key rotation from silently breaking in-place upgrades.

The private release key must be preserved for future compatible updates. Losing it prevents normal signed upgrades for users who installed an earlier APK under the same package ID.

## Source identity

The exact Git commit referenced by the release tag is the authoritative source identity. Historical root-level per-file checksum inventories were tied to older snapshots and are not maintained as the v1 source-integrity mechanism.

The v1 release manifest must record the exact source commit used to produce the signed APK. See [`INTEGRITY.md`](INTEGRITY.md).

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
4. APK semantic, label and signature verification.
5. Stable signing-certificate verification.
6. Artifact copy to the ignored `dist/` directory.
7. APK SHA-256 generation.
8. A JSON manifest recording release version, source commit, package, ABI, label, APK hash and signing-certificate digest.

Expected v1.0.0 outputs:

```text
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
dist/SystemShock-Android-v1.0.0-release.json
```

## v1.0 manual acceptance already established

Before the v1.0 source freeze, the project owner reported a complete start-to-finish playthrough on the Retroid Pocket 5 and accepted the game as functioning correctly on the reference target.

That manual product gate does not replace the final machine gates below. It means no additional enhancement work is required to justify stable status.

## Required machine gates before publishing v1.0.0

A stable APK is publishable only after all of these are true:

1. GitHub Actions QA passes from the exact final v1.0.0 public source tree.
2. A fresh local checkout passes dependency bootstrap, QA build and APK verification.
3. The accepted source commit is fixed and recorded.
4. The signed release build is generated from that exact commit with the established release key.
5. `verify-apk.ps1 -Variant release` passes.
6. The APK reports package `com.rp5np.systemshock`, `versionCode 10000`, `versionName 1.0.0`, label `System Shock - Android`, ABI `arm64-v8a`, and non-debuggable release state.
7. The release certificate SHA-256 matches the established stable digest.
8. The release APK SHA-256 and JSON release manifest are retained as evidence.
9. Git tag `v1.0.0` points to the exact accepted source commit.
10. The GitHub release uses that tag and publishes the verified signed APK plus its SHA-256 file.
11. Public README/site/ModDB status is changed from pre-release/candidate to stable only after the signed artifact exists.

## Data boundary

Commercial System Shock data is never embedded in the APK, Git repository, source archives, release artifacts or CI artifacts. Users provide compatible data from a legally obtained copy separately through the first-run Android importer.
