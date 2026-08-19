# Architecture notes

## Rendering

The Android target builds Shockolate with `ENABLE_OPENGL OFF` for the game renderer. The game world remains an indexed 8-bit software render path; SDL/GLES is used for presentation.

### 4:3 baseline

- Engine/UI reference: 1024×768.
- Android physical surface remains platform-owned.
- Presentation avoids non-uniform stretching.

### Hor+ prototype

- Logical presentation: 1366×768.
- Legacy 1024×768 UI safe area centered at x=171.
- 3D software canvas gains horizontal coverage while preserving legacy vertical scale/FOV behavior.
- A private runtime flag used during development can restore 4:3 independently of HD experiments.

## HD asset boundary

The indexed renderer is central to the current HD decision:

- Edge-aware Scale4x for sprite/UI assets can select existing indexed source colors exactly.
- Bicubic WORLD/STATIC output introduces new RGB colors and therefore requires lossy palette quantization in the current renderer.
- The project therefore blocks WORLD/STATIC HD promotion pending a truecolor-vs-quantized decision.

## Android storage

On Android the native entry point switches to `SDL_AndroidGetInternalStoragePath()` and then uses the original relative resource paths. This keeps the original engine's `res/data` and `res/sound` assumptions while placing the user's data in app-private storage.

## Audio

The stable Android design uses one SDL_mixer-owned physical audio device. Music, SFX and auxiliary streams coexist through the mixer rather than competing physical devices.

## Input

SDL GameController provides the semantic controller boundary. The right stick is camera-look by default; View/Select changes it to legacy cursor interaction. Android IME text is kept on the text-input path rather than being synthesized as printable controller/keyboard events.
