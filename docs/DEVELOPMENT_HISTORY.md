# Development history

The internal phase labels are retained here as traceable engineering milestones rather than exposed as unexplained status badges on the front page. Research milestones are historical evidence; they are not automatically shipped features.

| Milestone | Result |
|---|---|
| P1.2 | Real game rendering observed on RP5; menu reached; display/audio/input gaps identified. |
| P1.3 | 1024x768 internal mode, SDL_mixer restoration and controller integration work. |
| P1.4 | Fixed-landscape policy and Android IME text-entry bridge. |
| P1.5 | Platform ownership boundary: Android surface preserved; one-device mixer architecture; controller profile consolidation. |
| P1.6 | Right-stick camera look validated; View/Select toggle retained for fine cursor mode. |
| P1.7 | Gameplay music clock rebased after abnormal host lateness; manual gameplay music QA passed. |
| P1.8 | Native extraction of supported graphics resources using engine code; 8,870 frames extracted with zero unsupported frames in the validated set. Research only. |
| P1.9 | Offline HD method benchmark (nearest, Scale4x, bicubic) by asset class. Research only. |
| P2.0A | Reversible 1366x768 Hor+ prototype evaluated; not promoted to the stable preservation release. |
| P2.0B | Indexed-renderer compatibility study for HD experiments. Research only. |
| P2.0B-S2 | Sprite/UI runtime-HD experiment failed the manual quality gate; HD disabled and not promoted to the stable baseline. |
| P2.0D research | Offline font reconstruction; 36 fonts / 5,696 glyphs validated; experimental stages rejected rather than promoted. |
| v0.1.0-pre.3 | First APK-bearing public pre-release with stable signing identity and first-run user-facing data importer. |
| v1.0 scope freeze | Preservation-first product boundary: 1024x768 4:3 only; no HD, Hor+, font remaster, truecolor or gameplay/content changes in the stable line. |
| v1.0 manual acceptance | Project owner reports a complete start-to-finish playthrough on the Retroid Pocket 5 and accepts gameplay on the reference target. |

## Stable implementation baseline

The stable v1.0 line uses the validated 1024x768 4:3 no-stretch path together with the Android controller, IME, audio and first-run data-import adaptations required to access the original game on the reference handheld.

Hor+, HD and font work remain documented as research history and are explicitly outside the stable preservation scope. See [`PRESERVATION_SCOPE.md`](PRESERVATION_SCOPE.md).
