# Known issues and open concerns

This file deliberately separates unresolved work from already-proven behavior. Stable v1.0 follows the preservation boundary in [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md); enhancement research outside that boundary is not a release blocker.

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
**Resolution:** The proprietary-data rule is anchored to `/res/`, while Android resources are tracked normally.  
**Validation:** Fresh build, semantic APK verification and compiled launcher-icon verification passed after the correction.

## REL-SS-004 - End-user game-data importer

**Status:** Fixed and hardware validated.  
**Previous limitation:** The developer/QA flow required ADB and `run-as` to place commercial data in app-private storage, which is unsuitable for a normal non-debuggable public APK.  
**Resolution:** The Android launcher uses the Storage Access Framework to let the game owner select a compatible `res` folder containing `data` and `sound`. Files are copied into app-private storage through a staging directory and are activated only after the import completes successfully.  
**Validation:** A signed, non-debuggable side-by-side release-QA build was validated on the Retroid Pocket 5. The importer reported PASS, the game launched and remained alive through the smoke-test interval, the baseline package remained unchanged, and temporary QA data were removed afterward.

## REL-SS-005 - Public release signing identity

**Status:** Fixed and promoted to a stable-release invariant.  
**Resolution:** A dedicated release keystore is maintained outside the Git repository. The public package remains `com.rp5np.systemshock`.  
**Release certificate SHA-256:** `7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409`.  
**v1.0 hardening:** `scripts/verify-apk.ps1` pins that public certificate digest for release variants, so a stable APK signed with a different key fails verification even when no environment override is provided.  
**Operational requirement:** Preserve the private keystore and its credentials securely. They must never be committed to Git or included in public artifacts.

## REL-SS-006 - v0.1.0-pre.3 hardware/release gate

**Status:** Passed and published historically.  
**Validation:** Clean source checkout, pinned dependency bootstrap, JDK 17 QA build, semantic APK verification, side-by-side RP5 installation, game-data transfer/import validation, runtime smoke testing, manual gameplay approval, importer QA and baseline preservation passed for the pre-release line.  
**Release consequence:** `v0.1.0-pre.3` became the first APK-bearing public pre-release and established the signing identity carried forward to v1.0.

## REL-SS-007 - Stable v1.0 preservation scope and complete-playthrough acceptance

**Status:** Product/manual gate passed; machine release gate in progress.  
**Decision:** Stable v1.0 preserves the original game presentation/content and limits Android-specific changes to platform access and reliability.  
**Manual validation:** The project owner reports a complete start-to-finish playthrough on the Retroid Pocket 5 and accepts the game as working correctly on the reference target.  
**Release consequence:** Additional HD, Hor+, font-remaster, truecolor or gameplay-change work is not required before v1.0. See [`V1_RELEASE_GATE.md`](V1_RELEASE_GATE.md).

## OPEN-SS-002 - Broad Android compatibility

**Status:** Open / non-blocking for v1.0.  
**Impact:** The Retroid Pocket 5 is the validated reference device, but other Android devices may differ in controller mapping, OEM overlays, lifecycle behavior and display handling.  
**v1.0 rule:** Stable status applies to the documented RP5 reference boundary. Other Android 13+ ARM64 devices remain best-effort/unvalidated until compatibility reports exist.  
**Required follow-up:** Build the device compatibility matrix from independent reports; do not claim universal Android compatibility.

## SCOPE-SS-003 - WORLD/STATIC HD and truecolor path

**Status:** Closed / out of scope for the preservation release.  
**Historical finding:** Bicubic WORLD/STATIC remasters generate colors that cannot be represented exactly by the original indexed renderer without quantization or a different renderer.  
**Final stable decision:** Keep original resources authoritative. No HD WORLD/STATIC or truecolor renderer work is required for v1.0.

## SCOPE-SS-004 - Font remaster

**Status:** Closed / out of scope for the preservation release.  
**Historical evidence:** Offline research analysed 36 fonts and 5,696 glyphs and rejected several experimental stages.  
**Final stable decision:** Stable v1.0 uses the original game fonts. Font reconstruction remains historical research only.
