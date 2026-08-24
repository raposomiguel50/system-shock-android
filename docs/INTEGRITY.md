# Source and release integrity

## Purpose

This project separates **source identity** from **release-artifact identity**. A changing source tree should not rely on a static checksum list that silently becomes stale after later commits.

## v1 source identity

For the stable v1 line, the authoritative source identity is the exact Git commit referenced by the release tag.

Published `v1.0.0` is identified by:

- tag: `v1.0.0`;
- source commit: `d40e02b00e5e59b956b18fdd2a13a41672090b2c`;
- package: `io.github.raposomiguel50.systemshock`;
- APK SHA-256: `a6dcb7f76374dd7d4f7e39f0ecc08446f2956a33bda7655d699d5ba639b8526f`;
- signing certificate SHA-256: `806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`.

The release manifest, `.sha256` file and final-gate report record the same release identity. The public APK was downloaded again after publication and reproduced the expected APK SHA-256.

Git history provides integrity and provenance for source files. GitHub-generated source archives for the tag are therefore tied to the tagged commit rather than to a separately maintained per-file checksum inventory.

## Historical source checksum files

Earlier public snapshots included root-level per-file checksum inventories. Those files remain in Git history but are not the integrity authority for stable v1.

## Stable package/signing identity

Stable v1 uses package:

`io.github.raposomiguel50.systemshock`

The historical pre-release used:

`com.rp5np.systemshock`

The separate package ID intentionally removes the stable line's dependency on the historical pre-release signing key and lets both applications coexist.

The dedicated stable signing identity is now established. Every compatible future stable update must preserve certificate SHA-256:

`806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`

The private key and its credentials remain outside Git and public release artifacts.

## Release artifact identity

`scripts/release-gate.ps1` produces:

```text
SystemShock-Android-v1.0.0-arm64-v8a.apk
SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
SystemShock-Android-v1.0.0-release.json
```

`scripts/v1-final-gate.ps1` additionally produces:

```text
SystemShock-Android-v1.0.0-final-gate.txt
```

The JSON manifest records source commit, package, version, ABI, application label, APK SHA-256, signing-certificate SHA-256 and `proprietaryGameDataIncluded=false`.

`scripts/verify-apk.ps1` requires `RP5NP_RELEASE_CERT_SHA256` for release/release-QA variants and rejects APKs that do not match the explicitly pinned stable identity.

## Commercial-data boundary

The APK verifier checks for obvious embedded System Shock commercial-resource paths/files. This is a release guard, not a grant of rights to game data. Users supply compatible data separately from a lawful copy.

## Reproducibility boundary

The reproducibility claim applies to documented public source, pinned dependencies, toolchain and build behavior. The private stable signing key is deliberately not reproducible or public; it is an identity credential used only to produce official compatible stable updates.
