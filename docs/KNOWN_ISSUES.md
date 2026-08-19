# Known issues and open concerns

This file deliberately separates unresolved work from already-proven behavior.

## REL-SS-001 - v0.1.0-pre.1 SDL_mixer bootstrap failure

**Status:** Fixed on `main`; `v0.1.0-pre.1` remains affected  
**Observed during:** First clean-clone reproduction test on Windows 11 / PowerShell 7.6.4.  
**Symptom:** `scripts/bootstrap-deps.ps1` cloned SDL_mixer but failed at `git checkout --detach release-2.8.1`.  
**Root cause:** The helper cloned SDL_mixer with `--no-tags`, then attempted to check out the tag without explicitly fetching it.  
**Resolution on main:** If a requested ref is not locally resolvable, the helper explicitly fetches the matching tag before checkout. SDL_mixer `release-2.8.1` is also pinned to commit `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Release consequence:** Do not treat `v0.1.0-pre.1` as a passing fresh-clone reproduction release. A corrected pre-release must pass the clean-clone gate before superseding it.

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

**Status:** Open  
**Evidence:** Source checkout passed. The first dependency-bootstrap attempt exposed REL-SS-001 before the build gate was reached.  
**Required resolution:** A corrected public pre-release must pass dependency bootstrap, APK build, semantic APK verification, device installation, separately owned game-data import and RP5 runtime QA from a clean path.
