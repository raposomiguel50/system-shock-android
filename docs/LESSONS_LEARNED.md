# Lessons learned and engineering standards

These are intentionally written as reusable rules rather than a chronological diary.

## Platform ownership

- Treat the Android physical `Surface` as platform-owned and the game's logical render dimensions as engine-owned.
- Do not apply desktop resize assumptions directly to an SDL Android surface.
- Treat OEM overlays and Android focus/lifecycle transitions as a platform subsystem before diagnosing a renderer failure.

## Display and fidelity

- Preserve a known-good 4:3 baseline before extending presentation.
- Prefer reversible Hor+ expansion to non-uniform stretching.
- Keep original resources as a fallback/source of truth while experimental HD paths are evaluated.
- A sharper image is not automatically a better result; layout, anchors, transparency, palette identity and readability are part of fidelity.

## Input

- Use SDL GameController semantic actions rather than raw Android/Linux event codes.
- Keep camera look and legacy pointer interaction as separate modes when the original game needs both.
- Use `SDL_TEXTINPUT`/Android IME for committed text; do not reconstruct text from synthetic keyboard buttons.
- Keep editing/navigation keys on the key-event path and committed characters on the text-input path.

## Audio

- Do not open parallel physical audio devices for subsystems that must coexist.
- Use one mixer-owned physical device and route music, SFX and auxiliary streams through it.
- A real-time music scheduler should not drain overdue events in a burst after a host scheduling stall; rebase timing while preserving relative musical intervals.

## Build and automation

- Pin external source baselines and verify hashes/commits before expensive builds.
- A zero exit code is not sufficient evidence; verify the expected artifact and inspect its semantics.
- Compile the smallest changed native target first when isolating C/C++ failures, then link/package the complete target.
- Keep generated Gradle/CMake output outside source-control scope.
- On PowerShell, variable names are case-insensitive; never create semantically different variables that differ only by case.
- A script with a parse error cannot validate its own parser contract. Validate critical runners externally before mutation/execution.

## Evidence and QA

- Keep machine gates and manual visual/audio/control approval distinct.
- Real-device measurements are authoritative for target performance/behavior claims.
- Preserve failed experiments and root causes so they are not accidentally repeated.
- A portable handover/archive needs path, traversal, UTF-8 and independent hash verification-not merely successful extraction on the machine that created it.

## Commercial-data boundary

- Keep user-owned commercial assets outside Git, source packages and APKs.
- Document how game owners can supply their own data instead of automating redistribution.
