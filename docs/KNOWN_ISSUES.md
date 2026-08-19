# Known issues and open concerns

This file deliberately separates unresolved work from already-proven behavior.

## REL-SS-001 - v0.1.0-pre.1 SDL_mixer bootstrap failure

**Status:** Fixed and confirmed by a new clean `v0.1.0-pre.2` dependency bootstrap; `v0.1.0-pre.1` remains affected  
**Observed during:** First clean-clone reproduction test on Windows 11 / PowerShell 7.6.4.  
**Symptom:** `scripts/bootstrap-deps.ps1` cloned SDL_mixer but failed at `git checkout --detach release-2.8.1`.  
**Root cause:** The helper cloned SDL_mixer with `--no-tags`, then attempted to check out the tag without explicitly fetching it.  
**Resolution:** If a requested ref is not locally resolvable, the helper explicitly fetches the matching tag before checkout. SDL_mixer `release-2.8.1` is pinned to commit `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Targeted validation:** Re-running the corrected helper against the already-downloaded clean-clone dependency directories succeeded with SDL at `5d249570393f7a37e037abf22cd6012a4cc56a71`, SDL_mixer at `171eb2d420d5643e4ee11514a06e04a41a463bbd`, `BOOTSTRAP_DEPS=PASS`, and `BOOTSTRAP_FIX_TARGETED=PASS`.  
**Clean pre.2 validation:** A completely new `v0.1.0-pre.2` clone was created at `C:\MGDL-FreshClone-Tests\system-shock-android-v0.1.0-pre.2_20260820_004357`. No `.deps` directory existed before execution. The helper downloaded both dependencies from scratch and completed with `FRESH_PRE2_BOOTSTRAP=PASS`; SDL resolved to `5d249570393f7a37e037abf22cd6012a4cc56a71` and SDL_mixer to `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Release consequence:** `v0.1.0-pre.1` is retained as affected historical evidence. `v0.1.0-pre.2` supersedes it for public reproduction work.

## REL-SS-002 - Build attempted with unsupported JDK 25

**Status:** Environment mismatch identified; build gate hardened on `main`  
**Observed during:** First APK build attempt from the clean `v0.1.0-pre.2` reproduction path on Windows 11 / PowerShell 7.6.4.  
**Symptom:** The build helper warned that JDK 17 was expected but continued with OpenJDK 25. Gradle 8.1.1 then failed during build-script semantic analysis with `Unsupported class file major version 69`.  
**Root cause:** The reproduction host exposed JDK 25 through `JAVA_HOME`, while this public build baseline requires JDK 17. The helper treated the mismatch as a warning instead of a hard precondition.  
**Resolution on main:** `scripts/build.ps1` now parses the Java major version and fails immediately unless it is exactly 17, with an explicit instruction to pass `-JavaHome` to a JDK 17 installation.  
**Validation required:** Re-run the exact `v0.1.0-pre.2` build using JDK 17. The release tag remains unchanged because its documented requirement already specifies JDK 17; the stronger fail-fast behavior is a usability improvement on `main`.

## OPEN-SS-001 - End-user game-data importer

**Status:** Open  
**Impact:** A normal public release cannot currently ask a game owner to select/import their commercial data through the UI.  
**Current workaround:** Debug APK + ADB + `run-as` using `scripts/install-debug.ps1`.  
**Required resolution:** Implement and validate an Android user-facing import flow without embedding proprietary data in the APK.

## OPEN-SS-002 - Broad Android compatibility

**Status:** Open  
**Impact:** The Retroid Pocket 5 is validated, but other Android devices may differ in controller mapping, OEM overlays, lifecycle behavior and aspect handling.  
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

## OPEN-SS-005 - Public release identity/signing

**Status:** Open  
**Impact:** The Android `versionName` in the engineering snapshot still carries an internal development label. Release signing/versioning and a stable public package identity must be defined before a consumer release.

## OPEN-SS-006 - Independent fresh-clone proof

**Status:** Open, partially passed  
**Passed evidence:** Exact `v0.1.0-pre.2` source checkout from a new path and from-scratch public dependency bootstrap both passed.  
**Current blocker:** The first APK build attempt used JDK 25 instead of the required JDK 17 and therefore did not reach a valid build test.  
**Still required:** APK build under JDK 17, semantic APK verification, device installation, separately owned game-data import and RP5 runtime QA from the clean reproduction path.
