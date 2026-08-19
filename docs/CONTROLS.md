# Controls

The current RP5 profile deliberately separates camera look from precise interaction with the original mouse-driven interface.

| Input | Behavior |
|---|---|
| Left stick | Movement / navigation |
| Right stick | Camera look by default |
| View / Select | Toggle right stick between camera look and fine UI cursor mode |
| Touchscreen | Optional pointer/mouse-style interaction |
| Android IME | Text entry for player-name and other text fields |
| L2 | Left-mouse style selection/drag path for the original interface |
| R2 | Right-mouse / attack path according to the active game state |

The source also contains the detailed controller design records in the Android snapshot. Device behavior should be validated through SDL GameController semantics rather than raw Linux/Android event codes.
