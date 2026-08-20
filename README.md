# System Shock - Android

An unofficial native Android/ARM64 adaptation of [Shockolate](https://github.com/Interrupt/systemshock), developed with the Retroid Pocket 5 as the primary reference device.

**Project website:** [Miguel's Game Dev Lab - System Shock Android](https://raposomiguel50.github.io/projects/system-shock-android/)  
**Reproduction guide:** [Build it yourself](https://raposomiguel50.github.io/projects/system-shock-android/reproduce/)  
**Engineering knowledge base:** [Problems, decisions, standards and lessons](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/)  
**Latest public source preview:** [v0.1.0-pre.2](https://github.com/raposomiguel50/system-shock-android/releases/tag/v0.1.0-pre.2)  
**Feedback and testing:** [Open a structured report](https://github.com/raposomiguel50/system-shock-android/issues/new/choose)

This public snapshot is intended to make the engineering work inspectable and reproducible. It contains the redistributable source and Android project, but **does not contain System Shock game data**. You must provide compatible game data from a legally obtained copy separately.

## Start here

- If you want to see what the port does and how it differs from the original PC interaction model, start with the [project page](https://raposomiguel50.github.io/projects/system-shock-android/).
- If you want to build it, use the [reproduction guide](https://raposomiguel50.github.io/projects/system-shock-android/reproduce/).
- If you are interested in the engineering work, failed experiments and reusable lessons, use the [knowledge base](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/).
- If you tested it on another Android device, submit a [compatibility report](https://github.com/raposomiguel50/system-shock-android/issues/new?template=compatibility_report.yml).

## Status

- Development state: **pre-release / active development**
- Latest public source preview: **[v0.1.0-pre.2](https://github.com/raposomiguel50/system-shock-android/releases/tag/v0.1.0-pre.2)**
- Fresh source checkout + dependency bootstrap: **PASS**
- APK fresh-clone build: **in validation**
- Validated reference device: **Retroid Pocket 5**
- Current presentation: **1024x768 4:3 only, no stretch**
- Architecture: **Android / ARM64 (`arm64-v8a`)**
- Upstream Shockolate baseline: [`4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690`](https://github.com/Interrupt/systemshock/commit/4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690)
- SDL baseline: **2.32.10**, commit [`5d249570393f7a37e037abf22cd6012a4cc56a71`](https://github.com/libsdl-org/SDL/commit/5d249570393f7a37e037abf22cd6012a4cc56a71)
- SDL_mixer: **2.8.1**, commit [`171eb2d420d5643e4ee11514a06e04a41a463bbd`](https://github.com/libsdl-org/SDL_mixer/commit/171eb2d420d5643e4ee11514a06e04a41a463bbd)
- Proprietary game data included: **No**

## What is implemented

- Native Android/ARM64 build rather than desktop emulation.
- 1024x768 4:3 no-stretch presentation.
- Retroid Pocket 5 controller integration using SDL GameController semantics.
- Right stick controls camera look by default; View/Select toggles fine cursor mode.
- Optional touchscreen pointer interaction.
- Android IME text entry for the original player-name workflow.
- SDL_mixer/ADLMIDI audio path with a host-stall-resistant gameplay music clock.
- Private-resource research tooling and HD experiments kept separate from redistributable source.

The game presentation is intentionally kept in 4:3. The Android IME keyboard shown during text entry is the only platform-specific visual addition to the original game presentation.

## Reproduce the build

1. Read [`docs/BUILD.md`](docs/BUILD.md).
2. Run `scripts/bootstrap-deps.ps1` to obtain the pinned public dependencies.
3. Run `scripts/build.ps1` to build a debug APK.
4. Run `scripts/verify-apk.ps1` to inspect the APK before installing it.
5. Read [`docs/GAME_DATA.md`](docs/GAME_DATA.md) and supply your own compatible `res/data` and `res/sound` content.
6. For the current developer/debug workflow, use `scripts/install-debug.ps1` to install the APK and copy your own data into app-private storage.

The current data-import workflow deliberately uses a **debug build + ADB**. A user-friendly release importer is still an open item; see [`docs/KNOWN_ISSUES.md`](docs/KNOWN_ISSUES.md).

## Feedback, testing and contributions

Feedback is part of the project. Use the structured GitHub forms so reports remain searchable and useful to other testers:

- [Report a bug](https://github.com/raposomiguel50/system-shock-android/issues/new?template=bug_report.yml)
- [Suggest a feature or improvement](https://github.com/raposomiguel50/system-shock-android/issues/new?template=feature_request.yml)
- [Submit an Android compatibility report](https://github.com/raposomiguel50/system-shock-android/issues/new?template=compatibility_report.yml)
- [Report a documentation correction](https://github.com/raposomiguel50/system-shock-android/issues/new?template=documentation.yml)
- [Leave general feedback or ask a question](https://github.com/raposomiguel50/system-shock-android/issues/new?template=general_feedback.yml)
- [Open a pull request](https://github.com/raposomiguel50/system-shock-android/pulls)

For professional contact, use [LinkedIn](https://www.linkedin.com/in/miguel-raposo-7192a251/). Please do not attach proprietary System Shock game data, private information or credentials to public issues.

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for contribution and evidence guidelines.

## Knowledge base

The project documents not only successful changes but also failed experiments, root causes, operational concerns, validation evidence and reusable engineering rules. Start with the public [Engineering Knowledge Base](https://raposomiguel50.github.io/projects/system-shock-android/knowledge/) or [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md).

## Important boundary

This repository/source snapshot is code and documentation only. It does not grant rights to System Shock commercial assets and does not redistribute them. Keep your game data outside Git and outside redistributable archives.

## License

The source-port code is distributed under the GNU General Public License v3.0 or later, consistent with the upstream source. See [`LICENSE`](LICENSE) and [`COPYING.txt`](COPYING.txt). Third-party components retain their own notices and licenses; see [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).

## Project site

[Miguel's Game Dev Lab - System Shock Android](https://raposomiguel50.github.io/projects/system-shock-android/)
