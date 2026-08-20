# Known issues and open concerns

This file deliberately separates unresolved work from already-proven behavior.

## REL-SS-001 - v0.1.0-pre.1 SDL_mixer bootstrap failure

**Status:** Fixed and confirmed by clean dependency bootstrap testing.  
**Observed during:** First clean-clone reproduction test on Windows 11 / PowerShell 7.6.4.  
**Symptom:** `scripts/bootstrap-deps.ps1` cloned SDL_mixer but failed at `git checkout --detach release-2.8.1`.  
**Root cause:** The helper cloned SDL_mixer with `--no-tags`, then attempted to check out the tag without explicitly fetching it.  
**Resolution:** If a requested ref is not locally resolvable, the helper explicitly fetches the matching tag before checkout. SDL_mixer `release-2.8.1` is pinned to commit `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Validation:** Clean dependency bootstrap resolves SDL to `5d249570393f7a37e037abf22cd6012a4cc56a71` and SDL_mixer to `171eb2d420d5643e4ee11514a06e04a41a463bbd`, then reports `BOOTSTRAP_DEPS=PASS`.  
**Release consequence:** `v0.1.0-pre.1` is retained as affected historical evidence. `v0.1.0-pre.2` superseded it for public reproduction work.

## REL-SS-002 - Build attempted with unsupported JDK 25

**Status:** Fixed and validated.  
**Observed during:** First APK build attempt from a clean `v0.1.0-pre.2` checkout.  
**Symptom:** Gradle 8.1.1 failed with `Unsupported class file major version 69`.  
**Root cause:** The host exposed JDK 25 while the build baseline requires JDK 17.  
**Resolution:** The build path now has an explicit preflight and refuses any Java major version other than 17.  
**Validation:** JDK 17 fresh-clone builds and semantic APK verification have both passed.

## REL-SS-003 - Android launcher resources excluded by `.gitignore`

**Status:** Fixed and validated.  
**Symptom:** A clean checkout could reference `@mipmap/ic_launcher` while the Android resource directory was absent.  
**Root cause:** The repository rule `res/`, intended for proprietary game data at the repository root, also ignored `AndroidProject/app/src/main/res/`.  
**Resolution:** The proprietary-data rule is now anchored to `/res/`, while Android resources are tracked normally. The validated launcher resources are present in the public source tree.  
**Validation:** Fresh build, semantic APK verification and compiled launcher-icon verification passed after the correction.

## REL-SS-004 - End-user game-data importer

**Status:** Fixed and hardware validated for v0.1.0-pre.3.  
**Previous limitation:** The developer/QA flow required ADB and `run-as` to place commercial data in app-private storage, which is unsuitable for a normal non-debuggable public APK.  
**Resolution:** The Android launcher now uses the Storage Access Framework to let the game owner select a compatible `res` folder containing `data` and `sound`. Files are copied into app-private storage through a staging directory and are only activated after the import completes successfully.  
**Validation:** A signed, non-debuggable side-by-side `releaseQa` build was installed on the Retroid Pocket 5. The folder picker imported a temporary user-visible copy of the independently supplied game data, the importer reported PASS, the game launched and remained alive through the smoke-test interval, no fatal releaseQa logcat entry was detected, the baseline package remained unchanged, and the temporary package/test data were removed afterward.

## REL-SS-005 - Public release signing identity

**Status:** Fixed for v0.1.0-pre.3.  
**Resolution:** A dedicated release keystore was provisioned outside the Git repository. Public version metadata is `versionCode 13` and `versionName 0.1.0-pre.3`; the public package is `com.rp5np.systemshock`; the release variant is non-debuggable.  
**Release certificate SHA-256:** `7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`.  
**Operational requirement:** Preserve the private keystore and its credentials securely for future compatible upgrades. They must never be committed to Git or included in public artifacts.

## REL-SS-006 - Final v0.1.0-pre.3 RP5 hardware gate

**Status:** Passed.  
**Validation:** Clean source checkout, pinned dependency bootstrap, JDK 17 QA build, semantic APK verification, side-by-side RP5 installation, game-data transfer/import validation, runtime smoke testing, manual gameplay approval, importer QA and baseline preservation all passed.  
**Release consequence:** The importer changes were merged after successful CI and hardware validation. The final public release APK must be rebuilt and verified from the merged release commit before tagging and publication.

## OPEN-SS-002 - Broad Android compatibility

**Status:** Open.  
**Impact:** The Retroid Pocket 5 is the reference device, but other Android devices may differ in controller mapping, OEM overlays, lifecycle behavior and aspect handling.  
**Required resolution:** Build a device compatibility matrix from independent reports/tests.

## OPEN-SS-003 - WORLD/STATIC HD path

**Status:** Blocked by renderer architecture.  
**Finding:** The current game renderer is indexed 8-bit software rendering. Bicubic WORLD/STATIC remasters generate colors that cannot be represented exactly in the original palette.  
**Current rule:** Keep original resources authoritative. Do not ship a lossy WORLD/STATIC replacement without an explicit quality gate.  
**Decision still required:** truecolor renderer path versus explicitly accepted indexed quantization.

## OPEN-SS-004 - Font remaster

**Status:** Research in progress.  
**Evidence:** The validated offline dataset contains 36 fonts and 5,696 glyphs.  
**Concern:** Font/resource IDs are not globally unique unless resource-file identity is part of the key.  
**Current rule:** Review contour/vector candidates offline before runtime integration; do not re-use rejected experimental stages blindly.
