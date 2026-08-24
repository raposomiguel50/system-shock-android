# System Shock - Android

An unofficial native Android/ARM64 adaptation of [Shockolate](https://github.com/Interrupt/systemshock), developed with the Retroid Pocket 5 as the validated reference device.

The stable project goal is preservation and access: run the original System Shock experience on Android without remastering or replacing the game's content. See [`docs/PRESERVATION_SCOPE.md`](docs/PRESERVATION_SCOPE.md).

**Project website:** [Miguel's Game Dev Lab - System Shock Android](https://raposomiguel50.github.io/projects/system-shock-android/)  
**Reproduction guide:** [Build it yourself](https://raposomiguel50.github.io/projects/system-shock-android/reproduce/)  
**Engineering knowledge base:** [Problems, decisions, standards and lessons](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/)  
**Preservation scope:** [What stable v1.0 changes — and deliberately does not change](docs/PRESERVATION_SCOPE.md)  
**v1.0 release gate:** [Stable-release checklist](docs/V1_RELEASE_GATE.md)  
**Development method:** [Human direction, AI assistance, handovers and evidence](docs/DEVELOPMENT_METHOD.md)  
**Latest published APK:** [v0.1.0-pre.3](https://github.com/raposomiguel50/system-shock-android/releases/tag/v0.1.0-pre.3)  
**Feedback and testing:** [Open a structured report](https://github.com/raposomiguel50/system-shock-android/issues/new/choose)  
**ModDB:** [System Shock - Android](https://www.moddb.com/mods/system-shock-android)  
**Press / external coverage:** [Coverage and project milestones](docs/PRESS.md)

This public source tree contains redistributable code and Android project files but **does not contain System Shock commercial game data**. You must provide compatible game data from a legally obtained copy separately.

## Current status

- Source state: **v1.0.0 release candidate / preservation scope frozen**
- Source version metadata: **`versionCode 10000`, `versionName 1.0.0`**
- Latest published APK until the final signed v1.0 artifact is produced: **v0.1.0-pre.3**
- Complete start-to-finish playthrough on the reference Retroid Pocket 5: **accepted by the project owner**
- First-run Android game-data importer: **validated on Retroid Pocket 5**
- Stable presentation: **1024x768 4:3 only, no non-uniform stretch**
- Architecture: **Android / ARM64 (`arm64-v8a`)**
- Android package: **`com.rp5np.systemshock`**
- Application label: **System Shock - Android**
- Stable signing certificate SHA-256: **`7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`**
- Upstream Shockolate baseline: [`4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690`](https://github.com/Interrupt/systemshock/commit/4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690)
- SDL baseline: **2.32.10**, commit [`5d249570393f7a37e037abf22cd6012a4cc56a71`](https://github.com/libsdl-org/SDL/commit/5d249570393f7a37e037abf22cd6012a4cc56a71)
- SDL_mixer: **2.8.1**, commit [`171eb2d420d5643e4ee11514a06e04a41a463bbd`](https://github.com/libsdl-org/SDL_mixer/commit/171eb2d420d5643e4ee11514a06e04a41a463bbd)
- Proprietary game data included: **No**

The source metadata has been moved to `1.0.0`, but stable `v1.0.0` is **not considered published until the exact source commit passes the final signed-release machine gates and the verified APK is attached to a matching GitHub release**.

## Preservation definition

Stable v1.0 is intentionally not a remaster.

The shipped line keeps the original game content and original-style 4:3 presentation authoritative. It does **not** require or ship:

- widescreen/Hor+;
- HD replacement graphics;
- truecolor-renderer conversion;
- remastered fonts;
- replacement music or sound effects;
- gameplay rebalance, new maps, story changes or other new content.

Earlier experiments in those areas remain documented as engineering history but are not release blockers.

Android-specific adaptations are limited to what is needed for practical access and reliable operation: native ARM64 build support, platform lifecycle/presentation, controller mapping, fine cursor input, optional touch pointer input, Android text entry, Android-compatible audio handling, first-run data import, and release/reproducibility tooling.

## What is implemented

- Native Android/ARM64 build rather than desktop emulation.
- 1024x768 4:3 no-stretch presentation.
- Retroid Pocket 5 controller integration using SDL GameController semantics.
- Right stick controls camera look by default; View/Select toggles fine cursor mode for the original mouse-oriented interface.
- Optional touchscreen pointer interaction.
- Android IME text entry for the original player-name workflow.
- SDL_mixer/ADLMIDI audio path with host-stall-resistant gameplay music timing.
- First-run Android Storage Access Framework importer for a legally obtained System Shock `res` folder containing `data` and `sound`.
- Import staging and rollback so an incomplete copy is not activated as live game data.
- Signed non-debuggable release path with a pinned stable signing identity.
- Machine verification for package ID, version, label, ABI, native libraries, proprietary-data absence, debuggable state and signing certificate.

The Android IME keyboard shown during text entry is a platform interface rather than a replacement game asset.

## Install and first run

The public APK does not contain commercial System Shock resources.

1. Install the signed APK from the GitHub release.
2. Make your legally obtained compatible System Shock `res` folder available on the Android device.
3. Launch **System Shock - Android**.
4. Choose **Select res folder** when prompted.
5. Select the `res` directory containing both `data` and `sound`.
6. Approve access. The app copies the required files into private storage and launches the game.

Normal users do not need ADB or developer tools. See [`docs/INSTALL.md`](docs/INSTALL.md) and [`docs/GAME_DATA.md`](docs/GAME_DATA.md).

## Compatibility boundary

The Retroid Pocket 5 is the validated reference target. The project owner reports a completed full playthrough and accepts the resulting gameplay on that device.

Other Android 13+ ARM64 devices may work, but are not presented as validated until tested. See [`docs/COMPATIBILITY.md`](docs/COMPATIBILITY.md).

## Reproduce the build

1. Read [`docs/BUILD.md`](docs/BUILD.md).
2. Run `scripts/bootstrap-deps.ps1` to obtain the pinned public dependencies.
3. Run `scripts/qa-gate.ps1` for the isolated QA build and verifier.
4. For the stable signed build, configure the release-signing variables described in [`docs/RELEASE.md`](docs/RELEASE.md) and run `scripts/release-gate.ps1`.
5. Read [`docs/GAME_DATA.md`](docs/GAME_DATA.md) for the commercial-data boundary and importer behavior.

The release APK and source tree do not redistribute System Shock commercial game data.

## Development attribution and AI assistance

This project is human-directed and AI-assisted.

I define the project goals, target platforms, preservation boundary, constraints, feature priorities, interaction model, acceptance criteria and final decisions. Creative and critical decisions, hardware interpretation, physical testing and final approval remain under my control.

I use ChatGPT extensively as a development assistant for calculations, code drafting/modification, refactoring, debugging support, PowerShell/build automation, repetitive repository work, technical analysis, documentation, reproducibility work and release engineering.

AI-generated code, proposals and documentation are not treated as authoritative automatically. They are reviewed against the intended design and tested where applicable; incorrect assumptions and rejected approaches are retained when they provide useful engineering knowledge.

The workflow itself is part of the project contribution: explicit checkpoints, versioned handovers, evidence, hashes, recovery rules and real-hardware approval are used to preserve project intent and make the work inspectable.

Read [`docs/DEVELOPMENT_METHOD.md`](docs/DEVELOPMENT_METHOD.md) for the full method and attribution statement.

## Feedback, testing and contributions

Use the structured GitHub forms so compatibility and problems remain searchable:

- [Report a bug](https://github.com/raposomiguel50/system-shock-android/issues/new?template=bug_report.yml)
- [Suggest a preservation/access improvement](https://github.com/raposomiguel50/system-shock-android/issues/new?template=feature_request.yml)
- [Submit an Android compatibility report](https://github.com/raposomiguel50/system-shock-android/issues/new?template=compatibility_report.yml)
- [Report a documentation correction](https://github.com/raposomiguel50/system-shock-android/issues/new?template=documentation.yml)
- [Leave general feedback or ask a question](https://github.com/raposomiguel50/system-shock-android/issues/new?template=general_feedback.yml)
- [Open a pull request](https://github.com/raposomiguel50/system-shock-android/pulls)

Please do not attach proprietary System Shock data, private information or credentials to public issues.

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for contribution and evidence guidelines.

## Knowledge base and history

The project documents successful changes, failed experiments, root causes, operational concerns, validation evidence and reusable engineering rules. Start with the public [Engineering Knowledge Base](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/) or [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md).

Historical Hor+, HD and font experiments are preserved in documentation without being promoted into the stable preservation baseline.

## Press and external coverage

Independent editorial coverage is recorded separately from the technical release state. The first recorded article is GenerationAmiga's 21 August 2026 feature, [“System Shock Android port brings the 1994 classic to handhelds”](https://www.generationamiga.com/2026/08/21/system-shock-android-port-brings-the-1994-classic-to-handhelds/).

See [`docs/PRESS.md`](docs/PRESS.md).

## Important boundary

This repository does not grant rights to System Shock commercial assets and does not redistribute them. Keep your game data outside Git and outside redistributable source archives.

## License

The source-port code is distributed under the GNU General Public License v3.0 or later, consistent with the upstream source. See [`LICENSE`](LICENSE) and [`COPYING.txt`](COPYING.txt). Third-party components retain their own notices and licenses; see [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
