# Preservation scope

## Purpose

System Shock - Android exists to preserve access to the original System Shock experience on modern Android/ARM64 hardware. The project is not a remaster, remake or content-modification project.

The Android port may change the platform boundary when Android requires it, but the shipped game presentation and gameplay remain intentionally conservative.

## Shipped v1.0 preservation baseline

The v1.0 line is defined by these rules:

- Keep the original game logic and content authoritative.
- Keep the public presentation at 1024x768 4:3 without non-uniform stretching.
- Do not replace original graphics, fonts, music, sound effects, maps, story content or gameplay data.
- Do not add widescreen/Hor+, HD assets, truecolor rendering, font remasters, gameplay rebalance or new content to the stable release.
- Do not bundle commercial System Shock game data. The user supplies compatible data from a legally obtained copy.
- Use the stable Android package `io.github.raposomiguel50.systemshock`.
- Treat the historical pre-release package `com.rp5np.systemshock` as a separate development line. The stable package can coexist with it rather than depending on its signing key.
- Generate and preserve one dedicated signing identity for the stable v1 line and all compatible future stable updates.

The clean package/signing boundary is intentional: the first stable release must not depend on recovering the historical pre-release keystore.

## Android adaptations that are in scope

Changes are permitted when they are necessary to make the original game practical and reliable on Android. Current examples are:

- native Android/ARM64 build and lifecycle integration;
- SDL presentation of the original-style 4:3 game surface;
- controller mapping for handheld controls;
- right-stick camera input plus a fine cursor mode for the original mouse-oriented interface;
- optional touchscreen pointer input;
- Android IME text entry where the original game expects typed text;
- Android-compatible audio plumbing and timing fixes;
- a Storage Access Framework importer that copies user-supplied compatible game data into app-private storage;
- build, signing, verification and reproducibility tooling.

These are platform-access adaptations, not intended changes to System Shock itself.

## Research that is explicitly not part of v1.0

Earlier development explored Hor+ presentation, HD scaling and font reconstruction. Those experiments remain documented as engineering history, but they are not part of the preservation release and are not release blockers.

The stable project does not need a truecolor renderer, remastered WORLD/STATIC resources, replacement fonts or a widescreen mode to reach v1.0.

## Validation boundary

The Retroid Pocket 5 is the reference hardware target. Before the v1.0 source freeze, the project owner reported completing the game from start to finish on the reference device and accepted the resulting gameplay as working correctly.

That complete-playthrough acceptance is a manual hardware validation. Machine-verifiable release gates remain separate: clean source build, APK verification, stable signing identity, artifact hashes and release/tag consistency.

Other Android 13+ ARM64 devices may work, but they remain outside the validated reference boundary until independently tested.

## Legal and attribution boundary

This is an unofficial source port based on Shockolate. The repository and APK do not grant rights to System Shock commercial data and do not redistribute it. Users must obtain compatible game data separately from a lawful source.
