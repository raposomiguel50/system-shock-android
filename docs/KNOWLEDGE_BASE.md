# Open knowledge base

This knowledge base is structured like an operational engineering record: **context → symptom → impact → root cause → resolution/decision → validation → reusable lesson**. Open concerns are kept visible instead of being rewritten out of the project history.

## KB-SS-001 - PowerShell variable case collision

**Status:** Resolved  
**Context:** Android launch automation.  
**Symptom:** A launch component became corrupted even though the source/build itself was valid.  
**Root cause:** PowerShell variable names are case-insensitive; `$Activity` and `$activity` were treated as the same variable.  
**Resolution:** Eliminate case-only variable distinctions and add automatic-variable/casing collision validation to critical runners.  
**Validation:** Subsequent automation separated build/runtime defects from launcher defects and no longer relied on case-only names.  
**Reusable lesson:** On PowerShell, casing is presentation-not identity.

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
**Problem:** System Shock needs both modern camera-look behavior and precise access to a mouse-oriented interface.  
**Resolution:** Right stick controls camera look by default. View/Select toggles into fine cursor mode and back again. Touch remains an optional pointer path.  
**Reusable lesson:** Preserve distinct interaction modes instead of forcing a single abstraction across gameplay and legacy UI.

## KB-SS-006 - Gameplay music catch-up burst after host stalls

**Status:** Resolved / manual QA passed  
**Symptom:** Gameplay music could transiently accelerate or lose layer timing after scheduler delay.  
**Root cause:** Overdue XMI events were drained rapidly against an absolute millisecond clock.  
**Resolution:** Use SDL's monotonic high-resolution performance counter; after abnormal lateness, shift all active layer origins together rather than bursting overdue events.  
**Reusable lesson:** Real-time media clocks should preserve intervals after stalls, not replay lateness at maximum speed.

## KB-SS-007 - Widescreen without stretch

**Status:** Validated prototype / current baseline  
**Problem:** Filling a 16:9 display must not distort the original 4:3 scene/UI.  
**Resolution:** 1366×768 Hor+ expands horizontal world coverage while keeping a centered 1024×768 legacy UI safe area. 4:3 remains a reversible fallback.  
**Reusable lesson:** Preserve a reversible reference presentation before widening scene geometry.

## KB-SS-008 - HD assets versus an indexed 8-bit renderer

**Status:** Partially resolved; WORLD/STATIC remains open  
**Finding:** Sprite/UI Scale4x can remain exact because it selects source palette colors. Bicubic WORLD/STATIC output creates new RGB colors that require lossy quantization in the current renderer.  
**Decision:** Permit reversible indexed sprite/UI research; keep WORLD/STATIC original until a truecolor-versus-quantized path passes an explicit quality gate.  
**Reusable lesson:** Choose remaster algorithms against the runtime color model, not only against offline screenshots.

## KB-SS-009 - Resource identity collisions

**Status:** Resolved data-model rule  
**Problem:** A compound resource reference cannot always be assumed globally unique across separate resource files.  
**Resolution:** Runtime matching must include the relevant resource namespace/geometry/type/flags; ambiguous matches fall back to original resources. Font/glyph analysis likewise uses composite identities including the resource file.  
**Reusable lesson:** Verify the scope in which an identifier is unique before treating it as a primary key.

## KB-SS-010 - Build evidence is more than exit code

**Status:** Standard  
**Rule:** A build is not considered successful solely because a process returns zero. The expected APK must exist, expected ARM64 libraries must be present, proprietary assets must be absent, and target behavior must be separated into machine and manual QA gates.  
**Reusable lesson:** Evidence should prove the artifact and behavior you actually care about.

## Template for future entries

See [`kb/COMM_CELL_TEMPLATE.md`](kb/COMM_CELL_TEMPLATE.md).
