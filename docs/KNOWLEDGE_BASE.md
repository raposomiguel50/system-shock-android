# Open knowledge base

This knowledge base is structured like an operational engineering record: **context -> symptom -> impact -> root cause -> resolution/decision -> validation -> reusable lesson**. Historical research remains visible even when it is outside the stable preservation scope.

## KB-SS-001 - PowerShell variable case collision

**Status:** Resolved  
**Context:** Android launch automation.  
**Symptom:** A launch component became corrupted even though the source/build itself was valid.  
**Root cause:** PowerShell variable names are case-insensitive; `$Activity` and `$activity` were treated as the same variable.  
**Resolution:** Eliminate case-only variable distinctions and add automatic-variable/casing collision validation to critical runners.  
**Validation:** Subsequent automation separated build/runtime defects from launcher defects and no longer relied on case-only names.  
**Reusable lesson:** On PowerShell, casing is presentation, not identity.

## KB-SS-002 - Android surface/lifecycle mistaken for renderer failure

**Status:** Resolved as a diagnostic standard; OEM behavior remains a platform concern  
**Context:** RP5 runtime bring-up.  
**Symptom:** The SDL surface could disappear shortly after native startup when Retroid/Android UI layers took focus.  
**Root cause:** Android lifecycle/focus and OEM Game Assistant/notification overlays can affect the surface independently of the renderer.  
**Resolution:** Log lifecycle/focus/surface evidence and treat platform ownership separately from logical rendering.  
**Reusable lesson:** Prove surface/focus state before redesigning graphics code.

## KB-SS-003 - Competing audio-device ownership

**Status:** Resolved architecture  
**Symptom:** Raw SDL audio could be audible while SDL_mixer reported the device as already open.  
**Root cause:** Multiple subsystems were competing for physical audio-device ownership.  
**Resolution:** Use one SDL_mixer physical device and route music, SFX and auxiliary streams through the mixer/postmix paths.  
**Reusable lesson:** Shared subsystems should share one explicit owner at the platform boundary.

## KB-SS-004 - Android text entry is not controller-key emulation

**Status:** Resolved  
**Problem:** Player-name entry was designed for desktop keyboard input.  
**Resolution:** Use Android IME through SDL text input; consume committed characters from `SDL_TEXTINPUT` while leaving navigation/editing keys on the key-event path.  
**Additional guard:** Do not log the entered name; log counts/state only.  
**Reusable lesson:** Text and key buttons are different input domains.

## KB-SS-005 - Right stick must serve two interaction models

**Status:** Resolved / validated on RP5  
**Problem:** System Shock needs both camera-look behavior and precise access to a mouse-oriented interface.  
**Resolution:** Right stick controls camera look by default. View/Select toggles into fine cursor mode and back again. Touch remains an optional pointer path.  
**Reusable lesson:** Preserve distinct interaction modes instead of forcing a single abstraction across gameplay and legacy UI.

## KB-SS-006 - Gameplay music catch-up burst after host stalls

**Status:** Resolved / manual QA passed  
**Symptom:** Gameplay music could transiently accelerate or lose layer timing after scheduler delay.  
**Root cause:** Overdue XMI events were drained rapidly against an absolute millisecond clock.  
**Resolution:** Use SDL's monotonic high-resolution performance counter; after abnormal lateness, shift all active layer origins together rather than bursting overdue events.  
**Reusable lesson:** Real-time media clocks should preserve intervals after stalls, not replay lateness at maximum speed.

## KB-SS-007 - 4:3 presentation retained as the stable baseline

**Status:** Final stable decision  
**Context:** A 16:9 expansion prototype was evaluated during development.  
**Decision:** The stable port does not expose a switchable widescreen mode. It retains the original-style 1024x768 4:3 no-stretch presentation as the public presentation mode.  
**Visual boundary:** The Android IME keyboard shown during text entry is the only platform-specific visual addition to the original game presentation.  
**Reusable lesson:** A technically workable presentation experiment does not need to become a shipped feature when fidelity is better served by the reference format.

## KB-SS-008 - HD assets versus an indexed 8-bit renderer

**Status:** Research closed for the stable preservation line  
**Finding:** Sprite/UI Scale4x can remain exact because it selects source palette colors. Bicubic WORLD/STATIC output creates new RGB colors that require lossy quantization in the current renderer.  
**Historical decision:** Experimental HD paths were evaluated but not promoted.  
**Stable decision:** Original graphics remain authoritative; HD WORLD/STATIC, truecolor rendering and replacement graphics are out of scope for v1.0.  
**Reusable lesson:** Choose remaster algorithms against the runtime color model, and do not let optional research become a blocker for a preservation release.

## KB-SS-009 - Resource identity collisions

**Status:** Resolved data-model rule  
**Problem:** A compound resource reference cannot always be assumed globally unique across separate resource files.  
**Resolution:** Runtime matching must include the relevant resource namespace/geometry/type/flags; ambiguous matches fall back to original resources. Font/glyph analysis likewise uses composite identities including the resource file.  
**Reusable lesson:** Verify the scope in which an identifier is unique before treating it as a primary key.

## KB-SS-010 - Build evidence is more than exit code

**Status:** Standard  
**Rule:** A build is not considered successful solely because a process returns zero. The expected APK must exist, expected ARM64 libraries must be present, proprietary assets must be absent, and target behavior must be separated into machine and manual QA gates.  
**Reusable lesson:** Evidence should prove the artifact and behavior you actually care about.

## KB-SS-011 - A no-tags clone cannot assume a release tag is locally available

**Status:** Resolved and clean-clone validated in `v0.1.0-pre.2`  
**Context:** First independent clean-clone dependency bootstrap for public pre-release `v0.1.0-pre.1`.  
**Symptom:** SDL cloned and pinned correctly, but SDL_mixer failed at `git checkout --detach release-2.8.1`.  
**Impact:** The source checkout gate passed, but the dependency-bootstrap gate failed and the release could not yet be called independently reproducible.  
**Root cause:** The helper used `git clone --no-tags` and later tried to check out `release-2.8.1` without explicitly fetching that tag. The ref therefore did not exist locally.  
**Resolution:** Keep `--no-tags` for controlled cloning, but test whether the requested ref resolves locally and explicitly fetch `refs/tags/<tag>` when required. Pin SDL_mixer `release-2.8.1` to commit `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Validation:** A targeted rerun passed first. A completely new `v0.1.0-pre.2` clone was then created in a new path with no pre-existing `.deps` directory. The helper downloaded both dependencies from scratch and completed with `FRESH_PRE2_BOOTSTRAP=PASS`, SDL at `5d249570393f7a37e037abf22cd6012a4cc56a71` and SDL_mixer at `171eb2d420d5643e4ee11514a06e04a41a463bbd`.  
**Reusable lesson:** Dependency pinning must verify object availability, not merely name a tag. A clean clone is the test that exposes hidden dependence on local Git metadata.

## KB-SS-012 - A preservation release needs a product boundary

**Status:** Standard / v1.0 decision  
**Context:** Development generated valid research branches for Hor+, HD resources and font reconstruction after the core Android port was already playable.  
**Risk:** Treating every experiment as required work would prevent a stable release even though the project's actual goal is preservation and practical Android access.  
**Decision:** Stable v1.0 freezes the original-style 4:3 presentation and original content. Android-specific work is limited to platform access, input, text, audio, data import, compatibility and release reliability.  
**Validation:** The project owner reports a complete start-to-finish RP5 playthrough and accepts the resulting game behavior.  
**Reusable lesson:** Define stable completion against the project's purpose, not against every technically possible enhancement.

## Template for future entries

See [`kb/COMM_CELL_TEMPLATE.md`](kb/COMM_CELL_TEMPLATE.md).
