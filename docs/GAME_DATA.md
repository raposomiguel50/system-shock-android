# Supplying your own System Shock game data

## What is not included

This repository and its APK do **not** include proprietary System Shock resources. Do not commit commercial game data to this repository and do not attach it to public issues, source archives or releases.

Shockolate expects compatible resources under a layout equivalent to:

```text
res/
  data/
  sound/
```

The upstream Shockolate baseline expects assets from a compatible original CD-ROM or System Shock: Enhanced Edition installation; the upstream baseline does not support the floppy-disk asset set.

## Public Android import path

On first launch, if compatible game data is not already present in the app's private storage, the launcher asks the user to select a `res` folder through Android's Storage Access Framework.

The selected directory must contain both `data` and `sound` directories.

The importer:

1. reads the selected tree through Android's document APIs;
2. copies `data` and `sound` into a temporary `res.importing` directory inside app-private storage;
3. verifies that both imported directories contain data;
4. preserves an existing live `res` directory as a temporary backup when necessary;
5. activates the completed import only after copying succeeds;
6. removes temporary staging/backup data after successful activation;
7. attempts to restore the previous live directory if activation fails.

The game then uses the original relative resource paths from the app-private working directory.

## Storage and permissions boundary

The public application does not need to bundle game data or request broad filesystem access. The user explicitly selects a folder and grants read access through the Android system picker.

The imported app-private copy exists for runtime convenience. Keep the legally obtained source installation or archival copy separately. Uninstalling an Android application can remove its private storage.

## Developer / ADB path

Debug and QA workflows can still import a compatible `res` directory through the development helpers, for example:

```powershell
pwsh -File .\scripts\install-qa.ps1 -GameRes 'D:\Path\To\Your\SystemShock\res'
```

That `run-as`/ADB path is a developer mechanism. It is not the normal end-user installation path for the stable release.

## Preservation rule

The Android project treats the user's original game data as authoritative. Stable v1.0 does not replace it with HD assets, remastered fonts, modified maps, replacement audio or other content changes. See [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md).
