# System Shock - Android

An unofficial native Android/ARM64 adaptation of [Shockolate](https://github.com/Interrupt/systemshock), developed with the Retroid Pocket 5 as the validated reference device.

The stable project goal is preservation and access: run the original System Shock experience on Android without remastering or replacing the game's content.

**Stable release:** [v1.0.0](https://github.com/raposomiguel50/system-shock-android/releases/tag/v1.0.0)  
**Direct APK:** [SystemShock-Android-v1.0.0-arm64-v8a.apk](https://github.com/raposomiguel50/system-shock-android/releases/download/v1.0.0/SystemShock-Android-v1.0.0-arm64-v8a.apk)  
**Project website:** [Miguel's Game Dev Lab - System Shock Android](https://raposomiguel50.github.io/projects/system-shock-android/)  
**Reproduction guide:** [Build it yourself](https://raposomiguel50.github.io/projects/system-shock-android/reproduce/)  
**Engineering knowledge base:** [Problems, decisions, standards and lessons](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/)  
**Preservation scope:** [docs/PRESERVATION_SCOPE.md](docs/PRESERVATION_SCOPE.md)  
**Release gate:** [docs/V1_RELEASE_GATE.md](docs/V1_RELEASE_GATE.md)  
**Development method:** [docs/DEVELOPMENT_METHOD.md](docs/DEVELOPMENT_METHOD.md)  
**ModDB:** [System Shock - Android](https://www.moddb.com/mods/system-shock-android)

This repository and its APK **do not contain System Shock commercial game data**. Users provide compatible data from a legally obtained copy separately.

## Stable v1.0.0 status

- Release state: **stable preservation release**
- Version: **1.0.0** (`versionCode 10000`)
- Architecture: **Android / ARM64 (`arm64-v8a`)**
- Stable package: **`io.github.raposomiguel50.systemshock`**
- Historical pre-release package: **`com.rp5np.systemshock`**
- Application label: **System Shock - Android**
- Stable presentation: **1024x768 4:3, no non-uniform stretching**
- Reference hardware: **Retroid Pocket 5**
- Complete start-to-finish reference-device playthrough: **accepted**
- First-run Android game-data importer: **validated**
- Commercial game data included: **No**

### Verified release identity

- Source commit: [`d40e02b00e5e59b956b18fdd2a13a41672090b2c`](https://github.com/raposomiguel50/system-shock-android/commit/d40e02b00e5e59b956b18fdd2a13a41672090b2c)
- APK SHA-256: `a6dcb7f76374dd7d4f7e39f0ecc08446f2956a33bda7655d699d5ba639b8526f`
- Signing certificate SHA-256: `806d9cb061de67aa6953cdac573bd917da6aa17625964c2898d23e226bd5323b`
- Release tag: [`v1.0.0`](https://github.com/raposomiguel50/system-shock-android/releases/tag/v1.0.0)

The stable package intentionally differs from the historical pre-release package. This removes any dependency on the old pre-release signing key and allows both applications to coexist. Future stable releases must keep the v1 stable package and signing identity.

## Preservation definition

Stable v1.0 is intentionally **not a remaster**. The original game content and original-style presentation remain authoritative.

The stable release does **not** add:

- widescreen/Hor+;
- HD replacement graphics;
- truecolor conversion;
- remastered fonts;
- replacement music or sound effects;
- gameplay rebalance;
- new maps, story content or other gameplay changes.

Android-specific work is limited to what is required for practical access and reliable operation.

## Implemented Android adaptations

- Native Android/ARM64 build rather than desktop emulation.
- 1024x768 4:3 no-stretch presentation.
- Retroid Pocket 5 controller integration using SDL GameController semantics.
- Right-stick camera look by default.
- View/Select toggle for fine cursor mode used by the original mouse-oriented interface.
- Optional touchscreen pointer input.
- Android IME text entry for keyboard-oriented fields.
- SDL_mixer/ADLMIDI audio path with Android timing/platform fixes.
- First-run Storage Access Framework importer for a compatible `res` folder containing `data` and `sound`.
- Staged import and rollback so incomplete copies are not activated as live game data.
- Signed non-debuggable release path with semantic APK verification.

## Install and first run

1. Download and install `SystemShock-Android-v1.0.0-arm64-v8a.apk` from the [v1.0.0 release](https://github.com/raposomiguel50/system-shock-android/releases/tag/v1.0.0).
2. Make your legally obtained compatible System Shock `res` folder available on the Android device.
3. Launch **System Shock - Android**.
4. Choose **Select res folder**.
5. Select the `res` directory containing both `data` and `sound`.
6. Approve access. The app imports the files into private app storage and launches the game.

Normal users do not need ADB or developer tools. See [docs/INSTALL.md](docs/INSTALL.md) and [docs/GAME_DATA.md](docs/GAME_DATA.md).

## Compatibility boundary

The Retroid Pocket 5 is the validated reference target and has an accepted complete playthrough. Other Android 13+ ARM64 devices may work, but are not presented as validated until independently tested. See [docs/COMPATIBILITY.md](docs/COMPATIBILITY.md).

## Reproduce the build

1. Read [docs/BUILD.md](docs/BUILD.md).
2. Run `scripts/bootstrap-deps.ps1` to obtain the pinned public dependencies.
3. Run `scripts/qa-gate.ps1` for the isolated QA build and verifier.
4. For an independently signed build, configure the release-signing variables described in [docs/RELEASE.md](docs/RELEASE.md) and run `scripts/release-gate.ps1`.
5. Read [docs/GAME_DATA.md](docs/GAME_DATA.md) for the commercial-data boundary and importer behavior.

The official v1.0.0 APK was produced from the exact source commit recorded above and passed the project's final machine release gate. The release also publishes its `.sha256`, JSON manifest and final-gate report.

## Development attribution and AI assistance

This project is human-directed and AI-assisted. I define the goals, target platforms, preservation boundary, constraints, feature priorities, interaction model, acceptance criteria and final decisions. Hardware interpretation, physical testing and final approval remain under my control.

ChatGPT is used extensively for calculations, code drafting/modification, refactoring, debugging support, PowerShell/build automation, repetitive repository work, technical analysis, documentation, reproducibility work and release engineering. AI-generated output is reviewed against the intended design and tested where applicable.

See [docs/DEVELOPMENT_METHOD.md](docs/DEVELOPMENT_METHOD.md).

## Feedback and contributions

Use the repository's structured [issue forms](https://github.com/raposomiguel50/system-shock-android/issues/new/choose) for bugs, Android compatibility reports, documentation corrections and preservation/access suggestions. Pull requests are welcome where they respect the preservation boundary.

Do not attach proprietary System Shock data, private information or credentials to public issues.

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Knowledge base and history

The project documents successful changes, failed experiments, root causes, operational concerns, validation evidence and reusable engineering rules. Start with the public [Engineering Knowledge Base](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/) or [docs/KNOWLEDGE_BASE.md](docs/KNOWLEDGE_BASE.md).

Historical Hor+, HD and font experiments remain documented as engineering history without being promoted into the stable preservation baseline.

## Press

Independent coverage is recorded in [docs/PRESS.md](docs/PRESS.md). The first recorded article is GenerationAmiga's 21 August 2026 feature, “System Shock Android port brings the 1994 classic to handhelds”.

## License

The source-port code is distributed under the GNU General Public License v3.0 or later, consistent with upstream Shockolate. See [LICENSE](LICENSE), [COPYING.txt](COPYING.txt) and [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
