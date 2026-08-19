# Publish the sanitized source to a dedicated GitHub repository

The preferred public layout is a dedicated repository such as `system-shock-android`, rather than exposing the private development workspace or a handover archive.

## GitHub setup

Create an **empty public repository**. Do not initialize it with a README, `.gitignore` or license because those files already exist in this package.

Suggested repository name:

```text
system-shock-android
```

After extracting this public-source package, run from PowerShell 7:

```powershell
pwsh -File .\scripts\publish-to-github.ps1 -RepositoryUrl 'https://github.com/YOUR-ACCOUNT/system-shock-android.git'
```

The helper initializes the extracted package as a `main` Git repository, commits every public file, adds the remote and pushes it. It never reads from the private RP5 development workspace.

## Before announcing the repository

1. Confirm the source tree contains no commercial `res/data`, `res/sound` or `.res` assets.
2. Confirm `SOURCE_BASELINE.json` identifies the Shockolate/SDL baselines.
3. Confirm the GPL license and third-party notices are present.
4. Run the clean-clone reproducibility sequence in `BUILD.md`.
5. Record the successful clone/build/APK hash and tested device in the knowledge base.
