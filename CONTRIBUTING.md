# Contributing

Contributions, testing reports, corrections and technical suggestions are welcome.

## Choose the right channel

Use the structured issue forms whenever possible:

- Bug reports: reproduce a concrete failure and include exact observed evidence.
- Feature or improvement suggestions: explain the need, proposed change and relevant tradeoffs.
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

- installation and launch
- 4:3 presentation
- Hor+ presentation
- left-stick movement
- right-stick camera look
- View/Select cursor toggle
- L2/R2 interaction
- touchscreen pointer input
- Android text input
- music and sound effects

State `Not tested` rather than guessing.

## Pull requests

Keep changes focused and explain:

- what problem is being addressed
- what changed
- how it was tested
- what remains untested
- whether the change affects 4:3 fallback, Hor+, controller behavior, audio, Android lifecycle or game-data handling

Do not include generated build trees, commercial resources or private development infrastructure.

## Project boundaries

The repository contains redistributable source and documentation only. Users must supply compatible System Shock game data from a legally obtained copy separately.

## Professional contact

For professional networking or private professional enquiries, use Miguel's LinkedIn profile:

https://www.linkedin.com/in/miguel-raposo-7192a251/
