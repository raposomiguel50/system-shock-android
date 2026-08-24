# Release process

## Purpose

The stable release path is separate from normal development and QA. A public APK must not be produced from an arbitrary dirty workspace or the generic Android debug signing key.

Stable v1.0 is a preservation release. Its product boundary is defined in [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md), its integrity model in [`INTEGRITY.md`](INTEGRITY.md), and its final checklist in [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md).

## Stable application identity

Stable v1.0 starts a clean Android application/signing line:

- stable package: `io.github.raposomiguel50.systemshock`;
- historical pre-release package: `com.rp5np.systemshock`;
- versionCode: `10000`;
- versionName: `1.0.0`;
- application label: `System Shock - Android`;
- ABI: `arm64-v8a`.

Because the package IDs differ, the stable v1 APK can coexist with the historical pre-release. Stable v1 does not depend on recovering the pre-release keystore.

The Java/Android source namespace remains `com.rp5np.systemshock` internally; `applicationId` is the public install identity.

## Build identities

- `debug`: `io.github.raposomiguel50.systemshock`, generic debug key;
- `qa`: `io.github.raposomiguel50.systemshock.qa`, generic debug key;
- `releaseQa`: `io.github.raposomiguel50.systemshock.releaseqa`, stable release key;
- `release`: `io.github.raposomiguel50.systemshock`, non-debuggable, stable release key.

## Stable signing boundary

The first v1.0 release creates a new dedicated signing identity for the stable package. The key and its credentials remain outside Git.

The official one-click release runner creates the key automatically when it does not yet exist, generates a strong random password, stores the key in the project's private local signing area, and protects the generated credentials for the current Windows user. Later stable releases reuse that same identity automatically.

The build scripts receive the signing material through:

- `RP5NP_RELEASE_STORE_FILE`
- `RP5NP_RELEASE_STORE_PASSWORD`
- `RP5NP_RELEASE_KEY_ALIAS`
- `RP5NP_RELEASE_KEY_PASSWORD`
- `RP5NP_RELEASE_CERT_SHA256`

`RP5NP_RELEASE_CERT_SHA256` is mandatory for release/release-QA verification. The first stable release records the generated certificate digest in the release manifest and release evidence. Every future stable release must reuse that certificate.

## Source identity

The exact Git commit referenced by the release tag is the authoritative source identity. The release manifest records that same commit.

## Final release gate

The normal low-level final gate is:

```powershell
pwsh -File .\scripts\v1-final-gate.ps1
```

For the official stable build, use the project one-click runner supplied with the release workflow. It prepares or reuses the private stable signing identity, sets the required signing environment, clones the accepted source commit, executes the final gate and produces a feedback bundle without exposing credentials.

Expected outputs:

```text
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
dist/SystemShock-Android-v1.0.0-release.json
dist/SystemShock-Android-v1.0.0-final-gate.txt
```

## Required machine gates before publishing v1.0.0

1. GitHub Actions QA passes from the exact final v1.0.0 public source tree.
2. The one-click final runner creates or reuses the stable private signing identity and `scripts/v1-final-gate.ps1` passes from a clean checkout.
3. The accepted source commit is fixed and recorded.
4. `verify-apk.ps1 -Variant release` passes.
5. The APK reports package `io.github.raposomiguel50.systemshock`, `versionCode 10000`, `versionName 1.0.0`, label `System Shock - Android`, ABI `arm64-v8a`, and non-debuggable release state.
6. The generated stable signing-certificate SHA-256 is recorded and preserved for future stable upgrades.
7. The release APK SHA-256, JSON release manifest and final-gate report are retained as evidence.
8. Git tag `v1.0.0` points to the exact accepted source commit.
9. The GitHub release publishes the verified signed APK plus its SHA-256 file.
10. Public README/site/ModDB status changes to stable only after the signed artifact exists.

## Data boundary

Commercial System Shock data is never embedded in the APK, Git repository, source archives, release artifacts or CI artifacts. Users provide compatible data from a legally obtained copy separately through the first-run Android importer.
