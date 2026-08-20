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

## OPEN-SS-001 - End-user game-data importer

**Status:** Open  
**Impact:** A normal end-user build does not yet provide an Android UI for selecting/importing commercial game data.  
**Current developer/QA path:** ADB-based import into app-private storage using independently obtained game data.  
**Required resolution:** Implement and validate an Android user-facing import flow without embedding proprietary data in the APK.

## OPEN-SS-002 - Broad Android compatibility

**Status:** Open  
**Impact:** The Retroid Pocket 5 is the reference device, but other Android devices may differ in controller mapping, OEM overlays, lifecycle behavior and aspect handling.  
**Required resolution:** Build a device compatibility matrix from independent reports/tests.

## OPEN-SS-003 - WORLD/STATIC HD path

**Status:** Blocked by renderer architecture  
**Finding:** The current game renderer is indexed 8-bit software rendering. Bicubic WORLD/STATIC remasters generate colors that cannot be represented exactly in the original palette.  
**Current rule:** Keep original resources authoritative. Do not ship a lossy WORLD/STATIC replacement without an explicit quality gate.  
**Decision still required:** truecolor renderer path versus explicitly accepted indexed quantization.

## OPEN-SS-004 - Font remaster

**Status:** Research in progress  
**Evidence:** The validated offline dataset contains 36 fonts and 5,696 glyphs.  
**Concern:** Font/resource IDs are not globally unique unless resource-file identity is part of the key.  
**Current rule:** Review contour/vector candidates offline before runtime integration; do not re-use rejected experimental stages blindly.

## OPEN-SS-005 - Public release signing key

**Status:** Open  
**Resolved design:** Public version metadata is `versionCode 13` and `versionName 0.1.0-pre.3`. QA uses `com.rp5np.systemshock.qa`; release uses `com.rp5np.systemshock`. The release variant is non-debuggable and rejects the generic Android debug certificate.  
**Remaining requirement:** Provision and securely preserve the dedicated stable release keystore and record its certificate SHA-256 before producing the public APK.

## OPEN-SS-006 - Final v0.1.0-pre.3 hardware gate

**Status:** Open, build gates passed  
**Passed evidence:** Clean source checkout, pinned dependency bootstrap, JDK 17 build, semantic APK verification and baseline comparison have passed.  
**Remaining requirement:** Build the dedicated QA variant from the current public source, install it side-by-side on the Retroid Pocket 5, import independently obtained game data only into the QA package, and complete runtime QA without changing the existing baseline installation. After acceptance, generate and verify the signed release APK from the accepted commit.
