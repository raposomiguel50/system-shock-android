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
5. the APK signing certificate matches the established stable signing identity.

Git history provides integrity and provenance for the source files. GitHub-generated source archives for a tag are therefore tied to that tagged commit rather than to a separately maintained per-file checksum inventory.

## Historical source checksum files

Earlier public snapshots included root-level `SHA256SUMS.txt` and `PUBLIC_SOURCE_SHA256SUMS.txt` files containing per-file hashes for a specific source snapshot.

Those lists became stale as the public repository continued to evolve. Keeping them at the root of the current v1 source tree could incorrectly imply that they describe the current commit.

They are therefore removed from the current v1 branch and retained permanently in Git history as historical evidence. They are **not** the integrity authority for v1.0.0.

## Release artifact identity

`scripts/release-gate.ps1` produces:

```text
SystemShock-Android-v1.0.0-arm64-v8a.apk
SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
SystemShock-Android-v1.0.0-release.json
```

The JSON manifest records the source commit, package, version, ABI, application label, APK SHA-256, signing-certificate SHA-256 and the fact that proprietary game data is not included.

The stable signing certificate SHA-256 is:

`7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`

`scripts/verify-apk.ps1` pins this digest for release and release-QA variants. A differently signed APK must fail the stable release gate.

## Commercial-data boundary

The APK verifier also checks for obvious embedded System Shock commercial-resource paths/files. This is a release guard, not a grant of rights to any game data. Users supply compatible game data separately from a lawful copy.

## Reproducibility boundary

The reproducibility claim applies to the documented public source, pinned dependencies and toolchain. The private signing key is deliberately not reproducible or public; it is an identity credential used only to produce official compatible upgrades.
