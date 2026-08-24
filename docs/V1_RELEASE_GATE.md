# v1.0.0 release gate

This document tracks the transition from the public pre-release line to the first stable preservation release.

## Product definition

`v1.0.0` means the original System Shock experience is practically accessible on the validated Android/ARM64 reference target without requiring ADB or developer tools for normal installation and first run. It does not mean the project has added remaster features or broad device support.

See [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md) and [`INTEGRITY.md`](INTEGRITY.md).

## Accepted manual/product gates

- [x] Complete game playthrough on the Retroid Pocket 5 reported and accepted by the project owner.
- [x] 1024x768 4:3 no-stretch presentation accepted as the stable public presentation.
- [x] Handheld controller model accepted, including fine cursor access to the original interface.
- [x] Android text-entry path accepted.
- [x] Audio behavior accepted during gameplay.
- [x] First-run user-facing game-data importer validated on the Retroid Pocket 5.
- [x] No commercial System Shock data is embedded in the APK or public source.
- [x] HD, widescreen/Hor+, font-remaster and gameplay-change work declared out of scope for the stable preservation release.
- [x] Stale per-file checksum inventories retired from the current branch; exact Git commit + release manifest + APK SHA-256 + stable signing certificate define v1 integrity.

## Final local machine gate

Run the final candidate from a **clean checkout** with the established private release-signing environment configured:

```powershell
pwsh -File .\scripts\v1-final-gate.ps1
```

The wrapper runs the QA gate, produces the signed release through `release-gate.ps1`, verifies the stable APK identity and signing certificate, cross-checks the release manifest and `.sha256` file, and writes a final report into `dist/` without printing signing credentials.

Expected additional report:

```text
dist/SystemShock-Android-v1.0.0-final-gate.txt
```

## Machine/release gates

The following must be completed against the exact final v1.0.0 source commit before the GitHub release is published:

- [ ] GitHub Actions QA build and semantic APK verification pass after the final v1.0.0 source changes.
- [ ] `scripts/v1-final-gate.ps1` reports `V1_FINAL_LOCAL_GATE=PASS` from a clean checkout.
- [ ] Fresh local checkout resolves the pinned public dependencies.
- [ ] Signed `release` build is produced with the established stable key.
- [ ] `scripts/verify-apk.ps1 -Variant release` passes.
- [ ] Release certificate SHA-256 equals `7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`.
- [ ] APK SHA-256 and release manifest are recorded and agree with the final-gate report.
- [ ] Git tag `v1.0.0` points to the exact accepted source commit.
- [ ] GitHub release contains the signed APK and its SHA-256 file.
- [ ] Public README, project website and ModDB are updated from pre-release/candidate to stable only after the signed artifact is published.

## Version identity

- Version name: `1.0.0`
- Version code: `10000`
- Package: `com.rp5np.systemshock`
- ABI: `arm64-v8a`
- Application label: `System Shock - Android`
- Reference device: Retroid Pocket 5

`versionCode 10000` establishes a simple stable-version numbering base while remaining greater than the pre-release code `13`, so Android can treat the stable APK as an upgrade when the signing identity is unchanged.

## Stop condition

Do not publish or tag `v1.0.0` merely because the source metadata says `1.0.0`. The stable release exists only after the signed APK has passed the machine gates above.
