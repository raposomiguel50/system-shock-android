# Install and first run

## Public-user path

The stable Android path is designed so a normal user does not need ADB, USB debugging, PowerShell or the Android SDK.

### Requirements

- Android 13 or later on an ARM64 (`arm64-v8a`) device
- the signed System Shock - Android APK
- your own compatible System Shock `res` directory containing both `data` and `sound`

The Retroid Pocket 5 is the validated reference device. Other Android 13+ ARM64 devices are not guaranteed until tested; see [`COMPATIBILITY.md`](COMPATIBILITY.md).

## Install

1. Download the signed APK from the GitHub release.
2. Install it through Android's normal APK installation flow.
3. Make your legally obtained compatible System Shock `res` folder available on the device.
4. Launch **System Shock - Android**.
5. Choose **Select res folder**.
6. Select the `res` directory that contains `data` and `sound`.
7. Approve folder access.

The importer copies the selected data into the application's private storage. The game launches after the import completes successfully. Normal later launches go directly to the game while the imported data remains present.

No commercial System Shock game data is included in the APK.

## Import safety

The importer uses a staging directory. Existing live game data is not replaced until the new import has completed successfully. If activation fails, the importer attempts to restore the previous data directory rather than promoting a partial copy.

The public manifest does not request general Internet access or broad legacy storage permission for this process. Folder selection uses Android's Storage Access Framework.

## Stable v1 versus historical pre-release

Stable v1 uses package ID `io.github.raposomiguel50.systemshock`.

The historical public pre-release used `com.rp5np.systemshock`. Because these package IDs differ, Android treats them as separate applications and they may coexist. Stable v1 therefore does not require or depend on the historical pre-release signing key.

If both are installed, each has its own app-private storage. Import your legal `res` folder into the stable app on its first launch. Once the stable line is established, future stable releases reuse the same package and signing identity so normal in-place upgrades remain possible within the stable line.

Uninstalling an application may remove that application's private storage. Keep your legally obtained original `res` data available independently and do not treat the app-private imported copy as your archival master.

## Developer / QA path

The dedicated QA build uses `io.github.raposomiguel50.systemshock.qa`, so it can coexist with both the stable installation and the historical pre-release.

Build and verify it with:

```powershell
pwsh -File .\scripts\qa-gate.ps1
```

For developer-controlled ADB import into the QA package:

```powershell
pwsh -File .\scripts\install-qa.ps1 -GameRes 'D:\Path\To\res'
```

The QA/ADB path is not required for normal public use.
