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
- [x] Stable package separated from the historical pre-release so v1 no longer depends on recovering the pre-release signing key.

## Stable v1 identity

- Version name: `1.0.0`
- Version code: `10000`
- Stable package: `io.github.raposomiguel50.systemshock`
- Historical pre-release package: `com.rp5np.systemshock`
- ABI: `arm64-v8a`
- Application label: `System Shock - Android`
- Reference device: Retroid Pocket 5

The stable and historical pre-release packages can coexist. Stable v1 creates its own long-lived signing identity; that identity becomes fixed only after the first successful official stable build.

## Final local machine gate

The official one-click release runner must:

1. create the dedicated stable signing key automatically if it does not exist;
2. generate strong random signing credentials without requiring the user to choose a password;
3. store the key only in the private local project area;
4. protect the generated credentials for the current Windows user;
5. calculate the new signing-certificate SHA-256 and set it as the release verification pin;
6. clone the exact accepted source commit;
7. run `scripts/v1-final-gate.ps1`;
8. return the APK, APK SHA-256, JSON manifest, final-gate report and feedback bundle.

The low-level gate remains:

```powershell
pwsh -File .\scripts\v1-final-gate.ps1
```

## Remaining machine/release gates

- [ ] GitHub Actions QA build and semantic APK verification pass after the stable-package source change.
- [ ] Official one-click stable-signing/release runner reports PASS from a clean checkout.
- [ ] Fresh local checkout resolves the pinned public dependencies.
- [ ] Signed `release` build is produced with the newly established stable key.
- [ ] `scripts/verify-apk.ps1 -Variant release` passes against the generated certificate pin.
- [ ] New stable certificate SHA-256 is recorded in the release manifest and final-gate report.
- [ ] APK SHA-256 and release manifest are retained as evidence.
- [ ] Git tag `v1.0.0` points to the exact accepted source commit.
- [ ] GitHub release contains the signed APK and its SHA-256 file.
- [ ] Public README, project website and ModDB are updated from pre-release/candidate to stable only after the signed artifact is published.

## Stop condition

Do not publish or tag `v1.0.0` merely because the source metadata says `1.0.0`. The stable release exists only after the newly established stable signing identity and signed APK have passed the machine gates above.
