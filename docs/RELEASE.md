# Release process

## Purpose

The stable release path is separate from normal development and QA. A public APK must not be produced from an arbitrary dirty workspace or the generic Android debug signing key.

Stable v1.0 is a preservation release. Its product boundary is defined in [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md), its integrity model in [`INTEGRITY.md`](INTEGRITY.md), and its completed final checklist in [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md).

## Stable application identity

- stable package: `io.github.raposomiguel50.systemshock`;
- historical pre-release package: `com.rp5np.systemshock`;
- versionCode: `10000`;
- versionName: `1.0.0`;
- application label: `System Shock - Android`;
- ABI: `arm64-v8a`.

Because package IDs differ, stable v1 can coexist with the historical pre-release and does not depend on recovering the old pre-release keystore.

The Java/Android source namespace remains `com.rp5np.systemshock` internally; `applicationId` is the public install identity.

## Build identities

- `debug`: `io.github.raposomiguel50.systemshock`, generic debug key;
- `qa`: `io.github.raposomiguel50.systemshock.qa`, generic debug key;
- `releaseQa`: `io.github.raposomiguel50.systemshock.releaseqa`, stable release key;
- `release`: `io.github.raposomiguel50.systemshock`, non-debuggable, stable release key.

## Established stable signing identity

The dedicated stable signing identity was created during the v1.0.0 final release process and is now fixed for compatible future stable updates.

Signing certificate SHA-256:

`806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`

The private key and its credentials remain outside Git. Build scripts receive signing material through:

- `RP5NP_RELEASE_STORE_FILE`
- `RP5NP_RELEASE_STORE_PASSWORD`
- `RP5NP_RELEASE_KEY_ALIAS`
- `RP5NP_RELEASE_KEY_PASSWORD`
- `RP5NP_RELEASE_CERT_SHA256`

`RP5NP_RELEASE_CERT_SHA256` is mandatory for release/release-QA verification. Future official stable releases must reuse the established certificate above.

## Published v1.0.0 evidence

- Release tag: `v1.0.0`
- Release source commit: `d40e02b00e5e59b956b18fdd2a13a41672090b2c`
- Package: `io.github.raposomiguel50.systemshock`
- APK SHA-256: `a6dcb7f76374dd7d4f7e39f0ecc08446f2956a33bda7655d699d5ba639b8526f`
- Signing certificate SHA-256: `806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`
- Proprietary game data included: `false`

The public GitHub release contains:

```text
SystemShock-Android-v1.0.0-arm64-v8a.apk
SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
SystemShock-Android-v1.0.0-release.json
SystemShock-Android-v1.0.0-final-gate.txt
```

The published APK was downloaded again after release and its SHA-256 matched the accepted release hash.

## Low-level release gates

The signed release path remains available for later maintenance releases:

```powershell
pwsh -File .\scripts\release-gate.ps1
```

The v1-style full gate is:

```powershell
pwsh -File .\scripts\v1-final-gate.ps1
```

The gate checks static PowerShell validation, release preflight, build, APK semantics, package/version/ABI, non-debuggable state, stable signing certificate, artifact hashes and release-manifest consistency.

## Data boundary

Commercial System Shock data is never embedded in the APK, Git repository, source archives, release artifacts or CI artifacts. Users provide compatible data from a legally obtained copy separately through the first-run Android importer.
