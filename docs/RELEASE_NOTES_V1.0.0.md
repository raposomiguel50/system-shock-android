# System Shock - Android v1.0.0

First stable preservation release of the native Android/ARM64 System Shock port based on Shockolate.

## Stable baseline

- Native Android/ARM64 (`arm64-v8a`).
- Package: `io.github.raposomiguel50.systemshock`.
- Version: `1.0.0` (`versionCode 10000`).
- 1024x768 4:3 presentation without non-uniform stretching.
- Retroid Pocket 5 controller integration.
- Right-stick camera look plus fine cursor mode for the original mouse-oriented interface.
- Optional touchscreen pointer input.
- Android IME support for text entry.
- SDL_mixer/ADLMIDI audio path and Android timing/platform fixes required for reliable playback.
- First-run Storage Access Framework importer for a compatible user-supplied System Shock `res` folder containing `data` and `sound`.
- Staged import/rollback so incomplete game-data copies are not promoted as live data.
- Dedicated stable non-debuggable release signing identity.
- Reproducible source/build documentation and semantic APK verification.

## Preservation boundary

Stable v1.0 intentionally does **not** include:

- commercial System Shock game data;
- widescreen/Hor+;
- HD replacement graphics;
- truecolor renderer conversion;
- remastered fonts;
- replacement music or sound;
- gameplay, map, story or balance changes.

## Installation

1. Install `SystemShock-Android-v1.0.0-arm64-v8a.apk`.
2. Make a legally obtained compatible System Shock `res` folder available on the Android device.
3. Launch **System Shock - Android**.
4. Choose **Select res folder**.
5. Select the `res` directory containing both `data` and `sound`.
6. Approve access. The app imports the data into private storage and launches the game.

Normal end users do not need ADB or developer tools.

## Validation boundary

- Reference device: Retroid Pocket 5.
- Complete start-to-finish playthrough: accepted on the reference device.
- Stable presentation: 1024x768 4:3.
- Other Android 13+ ARM64 devices: potentially compatible but not claimed as validated until independently tested.

## Verified release identity

- Source commit: `d40e02b00e5e59b956b18fdd2a13a41672090b2c`
- Package: `io.github.raposomiguel50.systemshock`
- Version code: `10000`
- Version name: `1.0.0`
- Application label: `System Shock - Android`
- ABI: `arm64-v8a`
- APK SHA-256: `a6dcb7f76374dd7d4f7e39f0ecc08446f2956a33bda7655d699d5ba639b8526f`
- Signing certificate SHA-256: `806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`
- Proprietary game data included: `false`

The GitHub release also publishes the APK `.sha256`, JSON release manifest and final-gate report as machine-readable evidence.

## Package transition

The historical pre-release used `com.rp5np.systemshock`. Stable v1 uses `io.github.raposomiguel50.systemshock`, so both packages may coexist. The stable line has its own long-lived signing identity and future stable upgrades must preserve it.

## Legal boundary

This is an unofficial source port based on Shockolate. System Shock commercial game data is not included. Users must supply compatible data from a legally obtained copy separately.
