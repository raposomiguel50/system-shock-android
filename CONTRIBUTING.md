# Contributing

Contributions, testing reports, corrections and technical suggestions are welcome when they support the project's preservation and Android-access goals.

Stable releases are intentionally conservative. The original game content and 1024x768 4:3 presentation remain authoritative. Widescreen/Hor+, HD replacement assets, remastered fonts, gameplay changes and new content are outside the stable project scope. See [`docs/PRESERVATION_SCOPE.md`](docs/PRESERVATION_SCOPE.md).

## Choose the right channel

Use the structured issue forms whenever possible:

- Bug reports: reproduce a concrete failure and include exact observed evidence.
- Preservation/access suggestions: explain the Android access, compatibility, reliability, build or documentation need and the proposed change.
- Compatibility reports: successful tests are as valuable as failures. Include device, Android version and separate runtime checks.
- Documentation corrections: identify the page or file and provide the corrected information or supporting source.
- General feedback or questions: use the general feedback form when no other category fits.

Code changes should be submitted through a pull request.

## Evidence rules

1. Separate direct observation from inference.
2. Include the tested release, commit or source state where possible.
3. For build failures, include the failing command and relevant error output.
4. For device behavior, include device model, Android version and controller details when relevant.
5. A process exit code alone is not proof that the expected artifact or runtime behavior is correct.
6. Do not attach proprietary System Shock game data, credentials, private information or unrelated logs.

## Compatibility reports

When reporting runtime behavior, test these separately when practical:

- installation and first-run game-data import;
- menu/gameplay launch;
- 1024x768 4:3 presentation;
- left-stick movement;
- right-stick camera look;
- View/Select fine-cursor toggle;
- L2/R2 interaction;
- touchscreen pointer input;
- Android text input;
- music and sound effects;
- save/load behavior.

State `Not tested` rather than guessing.

## Pull requests

Keep changes focused and explain:

- what preservation/access problem is being addressed;
- what changed;
- how it was tested;
- what remains untested;
- whether the change affects original behavior, 4:3 presentation, controller input, audio, Android lifecycle, game-data handling, existing installations or reproducibility.

A stable-line pull request should not silently introduce replacement assets, widescreen/Hor+, remastered fonts, altered gameplay/content or commercial game data.

Do not include generated build trees, commercial resources, credentials or private development infrastructure.

## Project boundaries

The repository contains redistributable source and documentation only. Users must supply compatible System Shock game data from a legally obtained copy separately.

The Retroid Pocket 5 is the validated reference target. Compatibility improvements for other Android 13+ ARM64 devices are welcome when they preserve the stable fidelity boundary.

## Professional contact

For professional networking or private professional enquiries, use Miguel's LinkedIn profile:

https://www.linkedin.com/in/miguel-raposo-7192a251/
