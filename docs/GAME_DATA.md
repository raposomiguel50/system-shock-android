# Supplying your own System Shock game data

## What is not included

This source snapshot does **not** include proprietary System Shock resources. Do not commit them to this repository and do not attach them to source releases.

Shockolate expects compatible resources under a layout equivalent to:

```text
res/
  data/
  sound/
```

The upstream Shockolate documentation states that original CD-ROM or System Shock: Enhanced Edition assets are required; floppy-disk assets are not supported by that upstream baseline.

## Current Android storage model

On Android, the native entry point changes its working directory to the app's internal storage using `SDL_AndroidGetInternalStoragePath()`. The game then opens relative paths such as `res/data/...` and `res/sound/...`.

Therefore the current debug/developer workflow places your own `res` tree inside the app-private files directory.

## Debug/ADB import

After building a debug APK, connect an Android device with USB debugging enabled and run:

```powershell
pwsh -File .\scripts\install-debug.ps1 -GameRes 'D:\Path\To\Your\SystemShock\res'
```

`-GameRes` must point to the directory that contains both `data` and `sound`.

The script:

1. Installs/reinstalls the debug APK.
2. Copies your `data` and `sound` directories through a temporary ADB location.
3. Uses Android `run-as` to place them in this debug application's private `files/res` directory.
4. Removes the temporary copy.
5. Launches the application.

## Important limitation

`run-as` is a developer/debug mechanism. It is not the intended long-term distribution path for a normal signed release. A public end-user release should implement a user-facing import flow (for example through Android's document/storage APIs) so that game owners can select their own data without ADB.

That importer is tracked as an open engineering item rather than being hidden behind release packaging.
