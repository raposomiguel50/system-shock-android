# Compatibility matrix

Stable v1.0 is defined around one validated reference target rather than an unproven claim of universal Android compatibility.

| Target | Status | Evidence boundary |
|---|---|---|
| Retroid Pocket 5 | Validated v1.0 reference target | Project-owner complete playthrough accepted; 1024x768 4:3 presentation, built-in controls, fine cursor mode, Android text input, audio and first-run game-data import have been exercised on real hardware. |
| Other Android 13+ ARM64 handhelds | Unknown / unvalidated | The build targets ARM64 and minSdk 33, but controller mappings, OEM overlays, lifecycle behavior and display behavior may differ. Lack of broad-device validation is not a blocker for the RP5-defined v1.0 release. |
| Phones/tablets without physical controls | Partial by design | Touch can provide pointer interaction, but the stable port is designed around a physical-controller interaction model and is not validated as a touch-only product. |
| x86/x86_64 Android | Not built | The Android Gradle configuration filters to `arm64-v8a`. |

## Stable presentation boundary

The public v1.0 presentation is **1024x768 4:3 with no non-uniform stretching**. Earlier Hor+ and widescreen experiments are development history and are not part of the stable preservation release.

## Reporting compatibility

A useful compatibility report should include:

- device model and Android version;
- controller type/mapping;
- whether installation and the first-run data importer succeed;
- whether the game reaches menu and gameplay;
- 1024x768 4:3 display result;
- audio result;
- text-entry result;
- save/load observations when tested;
- any OEM game overlay or lifecycle interference;
- APK version and SHA-256 when available.

Use the repository's Android compatibility report form and do not attach proprietary game data.
