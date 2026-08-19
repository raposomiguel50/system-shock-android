# Compatibility matrix

| Target | Status | Evidence boundary |
|---|---|---|
| Retroid Pocket 5 | Validated reference target | Display, controls, IME, audio and Hor+ paths have been exercised on real hardware during development. |
| Other Android 13+ ARM64 handhelds | Unknown / unvalidated | The build targets ARM64 and minSdk 33, but controller mappings, OEM overlays and display behavior may differ. |
| Phones/tablets without physical controls | Partial by design | Touch can provide pointer interaction, but the current port is designed around a handheld controller and has not been validated as a touch-only product. |
| x86/x86_64 Android | Not built | The Android Gradle configuration currently filters to `arm64-v8a`. |

## Reporting compatibility

A useful compatibility report should include:

- device model and Android version;
- controller type/mapping;
- whether the game reaches menu/gameplay;
- display mode used (4:3 or Hor+);
- audio result;
- text-entry result;
- any OEM game overlay or lifecycle interference;
- APK/source snapshot identifier.
