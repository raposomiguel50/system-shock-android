# Publishing a public release

The dedicated public repository already exists at `raposomiguel50/system-shock-android`. Normal releases are published from the existing Git history; do not recreate the repository or publish from a private workspace snapshot.

## Stable-release source boundary

Before publishing `v1.0.0`:

1. Freeze the accepted public source commit on `main`.
2. Confirm the v1 preservation scope in [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md).
3. Complete the machine gates in [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md).
4. Build the signed release from a clean checkout of the exact accepted commit.
5. Retain the APK SHA-256, release JSON manifest and stable signing-certificate digest.
6. Create tag `v1.0.0` on that exact commit.
7. Publish the signed APK and its `.sha256` file through the GitHub release associated with that tag.
8. Copy the final source commit and APK SHA-256 into the release notes.
9. Only after the verified artifact is public, update the README, project website and ModDB from release-candidate/pre-release wording to stable `v1.0.0`.

See [`INTEGRITY.md`](INTEGRITY.md) for the source/release identity model.

## Required public artifacts

The release gate produces:

```text
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk
dist/SystemShock-Android-v1.0.0-arm64-v8a.apk.sha256
dist/SystemShock-Android-v1.0.0-release.json
```

The APK and `.sha256` file should be attached to the GitHub release. The JSON manifest should be retained as release evidence and may also be published when useful for verification.

## Commercial-data boundary

Before publication, confirm again that no commercial `res/data`, `res/sound`, `.res` files or other proprietary System Shock game data are present in the APK, repository additions or release attachments.

Users supply compatible game data separately from a legally obtained copy through the first-run Android importer.

## Signing boundary

The private release keystore and credentials never enter GitHub, source archives, CI artifacts or public release attachments. The stable public signing-certificate SHA-256 is documented and verified, but the private key remains private.

## Historical publication helper

`scripts/publish-to-github.ps1` was created for initial publication of a sanitized source package into an empty repository. It is retained as development history/utility and is **not** the normal path for current releases from the established repository.
