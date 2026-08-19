# Known issues and open concerns

This file deliberately separates unresolved work from already-proven behavior.

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
