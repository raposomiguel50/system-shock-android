# System Shock - Android

An unofficial native Android/ARM64 adaptation of [Shockolate](https://github.com/Interrupt/systemshock), developed with the Retroid Pocket 5 as the primary reference device.

This public snapshot is intended to make the engineering work inspectable and reproducible. It contains the redistributable source and Android project, but **does not contain System Shock game data**. You must provide compatible game data from a legally obtained copy separately.

## Status

- Development state: **pre-release / active development**
- Validated reference device: **Retroid Pocket 5**
- Current validated implementation baseline: **P2.0B + P2.0A Hor+**
- Architecture: **Android / ARM64 (`arm64-v8a`)**
- Upstream Shockolate baseline: [`4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690`](https://github.com/Interrupt/systemshock/commit/4cc3d07dfff2d11b6d3a0a9960a51cf4ca253690)
- SDL baseline: **2.32.10**, commit [`5d249570393f7a37e037abf22cd6012a4cc56a71`](https://github.com/libsdl-org/SDL/commit/5d249570393f7a37e037abf22cd6012a4cc56a71)
- SDL_mixer: **2.8.1**
- Proprietary game data included: **No**

## What is implemented

- Native Android/ARM64 build rather than desktop emulation.
- 1024×768 4:3 no-stretch baseline.
- Reversible 1366×768 Hor+ presentation with a centered 1024×768 UI safe area.
- Retroid Pocket 5 controller integration using SDL GameController semantics.
- Right stick controls camera look by default; View/Select toggles fine cursor mode.
- Optional touchscreen pointer interaction.
- Android IME text entry for the original player-name workflow.
- SDL_mixer/ADLMIDI audio path with a host-stall-resistant gameplay music clock.
- Private-resource research tooling and HD experiments kept separate from redistributable source.

## Reproduce the build

1. Read [`docs/BUILD.md`](docs/BUILD.md).
2. Run `scripts/bootstrap-deps.ps1` to obtain the pinned public dependencies.
3. Run `scripts/build.ps1` to build a debug APK.
4. Run `scripts/verify-apk.ps1` to inspect the APK before installing it.
5. Read [`docs/GAME_DATA.md`](docs/GAME_DATA.md) and supply your own compatible `res/data` and `res/sound` content.
6. For the current developer/debug workflow, use `scripts/install-debug.ps1` to install the APK and copy your own data into app-private storage.

The current data-import workflow deliberately uses a **debug build + ADB**. A user-friendly release importer is still an open item; see [`docs/KNOWN_ISSUES.md`](docs/KNOWN_ISSUES.md).

## Knowledge base

The project documents not only successful changes but also failed experiments, root causes, operational concerns, validation evidence and reusable engineering rules. Start with [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md).

## Important boundary

This repository/source snapshot is code and documentation only. It does not grant rights to System Shock commercial assets and does not redistribute them. Keep your game data outside Git and outside redistributable archives.

## License

The source-port code is distributed under the GNU General Public License v3.0 or later, consistent with the upstream source. See [`LICENSE`](LICENSE) and [`COPYING.txt`](COPYING.txt). Third-party components retain their own notices and licenses; see [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).

## Project site

[Miguel's Game Dev Lab - System Shock Android](https://raposomiguel50.github.io/projects/system-shock-android/)
