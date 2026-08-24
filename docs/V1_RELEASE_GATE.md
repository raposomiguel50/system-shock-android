# v1.0.0 release gate — complete

`v1.0.0` is the first stable preservation release of System Shock - Android.

## Product/manual gates

- [x] Complete start-to-finish playthrough accepted on the Retroid Pocket 5 reference device.
- [x] 1024x768 4:3 no-stretch presentation accepted.
- [x] Handheld controls and fine cursor accepted.
- [x] Android text entry accepted.
- [x] Audio behavior accepted.
- [x] First-run game-data importer accepted.
- [x] Commercial System Shock data remains outside the APK and public source.
- [x] HD, widescreen/Hor+, font-remaster, truecolor and gameplay/content changes remain outside stable-v1 scope.

## Machine/release gates

- [x] Public GitHub Actions QA passed after the stable-package transition.
- [x] Clean local final gate reported `V1_FINAL_LOCAL_GATE=PASS`.
- [x] Pinned public dependencies resolved from a fresh checkout.
- [x] Signed non-debuggable `release` APK produced and verified.
- [x] Stable package verified: `io.github.raposomiguel50.systemshock`.
- [x] Version verified: `1.0.0` / `versionCode 10000`.
- [x] ABI verified: `arm64-v8a`.
- [x] Stable signing certificate recorded.
- [x] APK SHA-256 recorded.
- [x] Release JSON, `.sha256` and final-gate report agree.
- [x] Tag `v1.0.0` points to the accepted release source commit.
- [x] GitHub Release `v1.0.0` is public with the signed APK and evidence files.
- [x] The publicly re-downloaded APK reproduced the expected SHA-256.

## Stable release identity

- Source commit: `d40e02b00e5e59b956b18fdd2a13a41672090b2c`
- Package: `io.github.raposomiguel50.systemshock`
- Version: `1.0.0` / `10000`
- ABI: `arm64-v8a`
- APK SHA-256: `a6dcb7f76374dd7d4f7e39f0ecc08446f2956a33bda7655d699d5ba639b8526f`
- Signing certificate SHA-256: `806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`
- Commercial game data included: `false`

The GitHub stable-release gate is complete. External mirrors or directories can be synchronized independently without changing the immutable `v1.0.0` source, APK or signing identity.
