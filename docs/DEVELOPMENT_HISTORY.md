# Development history

The internal phase labels are retained here as traceable engineering milestones rather than exposed as unexplained status badges on the front page.

| Milestone | Result |
|---|---|
| P1.2 | Real game rendering observed on RP5; menu reached; display/audio/input gaps identified. |
| P1.3 | 1024×768 internal mode, SDL_mixer restoration and controller integration work. |
| P1.4 | Fixed-landscape policy and Android IME text-entry bridge. |
| P1.5 | Platform ownership boundary: Android surface preserved; one-device mixer architecture; controller profile consolidation. |
| P1.6 | Right-stick camera look validated; View/Select toggle retained for fine cursor mode. |
| P1.7 | Gameplay music clock rebased after abnormal host lateness; manual gameplay music QA passed. |
| P1.8 | Native extraction of supported graphics resources using engine code; 8,870 frames extracted with zero unsupported frames in the validated set. |
| P1.9 | Offline HD method benchmark (nearest, Scale4x, bicubic) by asset class. |
| P2.0A | Reversible 1366×768 Hor+ prototype; geometry/UI/input and 4:3 fallback validated on RP5. |
| P2.0B | Indexed-renderer compatibility study: sprite/UI Scale4x exact in palette space; WORLD/STATIC bicubic requires lossy quantization. |
| P2.0B-S2 | Sprite/UI runtime-HD experiment failed manual quality gate; HD disabled and source not promoted to canonical baseline. |
| P2.0D research | Offline font reconstruction; 36 fonts / 5,696 glyphs validated; several experimental stages rejected rather than promoted. |

Current validated implementation baseline: **P2.0B + P2.0A Hor+**.
