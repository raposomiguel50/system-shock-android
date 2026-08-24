# Architecture notes

## Preservation boundary

The stable Android architecture adapts the platform around System Shock rather than redesigning the game. See [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md).

## Rendering

The Android target builds Shockolate with `ENABLE_OPENGL OFF` for the game renderer. The game world remains an indexed 8-bit software render path; SDL/GLES is used for presentation on Android.

### Stable 4:3 presentation

- Engine/UI reference: 1024x768.
- Android physical surface remains platform-owned.
- Presentation avoids non-uniform stretching.
- Stable v1.0 exposes the 4:3 path only.

### Historical presentation research

A 1366x768 Hor+ prototype was evaluated during development. It is retained as engineering history, not as a shipped stable feature. No widescreen/Hor+ mode is required for or included in the v1.0 preservation definition.

## Historical HD research

The indexed renderer constrained earlier HD experiments: Scale4x sprite/UI work could retain indexed source colors, while bicubic WORLD/STATIC output would require lossy palette quantization or a different renderer.

That research is closed for the stable preservation line. v1.0 keeps the original resources authoritative and does not require HD assets, truecolor rendering or remastered fonts.

## Android storage

On Android the native entry point switches to `SDL_AndroidGetInternalStoragePath()` and then uses the original relative resource paths. This keeps the engine's `res/data` and `res/sound` assumptions while placing the user's imported data in app-private storage.

The public launcher uses Android's Storage Access Framework to copy a user-selected compatible `res` directory through a staging-and-rollback process. Commercial game data is not embedded in the APK.

## Audio

The stable Android design uses one SDL_mixer-owned physical audio device. Music, SFX and auxiliary streams coexist through the mixer rather than competing for separate physical devices. Timing corrections at the Android/platform boundary are intended to preserve normal game audio behavior rather than alter content.

## Input

SDL GameController provides the semantic controller boundary. The right stick is camera-look by default; View/Select changes it to fine cursor interaction for the original mouse-oriented interface. Touch remains an optional pointer path.

Android IME text is kept on the text-input path rather than being synthesized as printable controller/keyboard events.

These input adaptations provide access to the original interaction model on handheld hardware; they do not change game logic or content.
