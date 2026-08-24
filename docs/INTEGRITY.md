# Source and release integrity

## Purpose

This project separates **source identity** from **release-artifact identity**. A changing source tree should not rely on a static checksum list that silently becomes stale after later commits.

## v1 source identity

For the stable v1 line, the authoritative source identity is the exact Git commit referenced by the release tag.

A published `v1.0.0` release is valid only when:

1. tag `v1.0.0` resolves to the accepted source commit;
2. the signed APK was built from that exact commit through the documented release gate;
3. the release manifest records the same commit;
4. the APK SHA-256 is published alongside the APK;
5. the APK signing certificate is the dedicated stable signing identity created for the stable package.

Git history provides integrity and provenance for the source files. GitHub-generated source archives for a tag are tied to that tagged commit rather than to a separately maintained per-file checksum inventory.

## Historical source checksum files

Earlier public snapshots included root-level per-file checksum inventories. Those files are retained in Git history but are not the integrity authority for stable v1.

## Stable package/signing identity

Stable v1 uses package:

`io.github.raposomiguel50.systemshock`

The historical pre-release used:

`com.rp5np.systemshock`

The separate package ID intentionally breaks the dependency on the historical pre-release signing key. The first official stable build creates a new dedicated private signing identity. Its certificate SHA-256 is generated before the release build, pinned into that build's verification environment, written into the release manifest/final-gate report and then preserved as the immutable signing identity for future stable updates.

The private key and credentials are never committed to Git.

## Release artifact identity

`scripts/release-gate.ps1` produces:

```text
SystemShock-Android-v1.0.0-arm64-v8a.apk
SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
SystemShock-Android-v1.0.0-release.json
```

The JSON manifest records the source commit, package, version, ABI, application label, APK SHA-256, signing-certificate SHA-256 and the fact that proprietary game data is not included.

`scripts/verify-apk.ps1` requires `RP5NP_RELEASE_CERT_SHA256` for release/release-QA variants and fails if the built APK does not match that explicitly pinned stable identity.

## Commercial-data boundary

The APK verifier checks for obvious embedded System Shock commercial-resource paths/files. This is a release guard, not a grant of rights to any game data. Users supply compatible game data separately from a lawful copy.

## Reproducibility boundary

The reproducibility claim applies to the documented public source, pinned dependencies and toolchain. The private stable signing key is deliberately not reproducible or public; it is an identity credential used only to produce official compatible stable updates.
