# Development method, attribution and AI assistance

## Human-directed, AI-assisted development

This project is directed by Miguel Raposo and developed with substantial assistance from ChatGPT.

The distinction matters. ChatGPT is used as a tool for implementation, analysis and repetitive technical work. It does not independently own the project, define its purpose, choose what should exist, perform physical hardware validation, or decide whether a result is acceptable.

Miguel Raposo defines the goals, design, target platforms, constraints, priorities, interaction model, presentation choices, project structure, acceptance criteria and final decisions. He provides the creative direction, critical judgment, hardware observations and approvals that determine what is implemented, changed, rejected or revisited.

ChatGPT helps reduce the amount of mechanical and time-consuming work required to execute those decisions. This includes calculations, code drafting and modification, refactoring, debugging support, PowerShell and build automation, repetitive repository work, technical comparison, documentation drafting, data transformation, reproducibility checks and release-engineering assistance.

The result should therefore be understood as Miguel Raposo's project, design direction and engineering judgment implemented with substantial AI assistance, not as an autonomously AI-generated project.

## Responsibility by area

| Area | Primary responsibility |
| --- | --- |
| Project concept and goals | Miguel Raposo |
| Creative direction and design | Miguel Raposo |
| Target hardware and platform choices | Miguel Raposo |
| Feature priorities and scope | Miguel Raposo |
| Controls, interaction and presentation decisions | Miguel Raposo |
| Project organization and working method | Miguel Raposo |
| Acceptance and rejection criteria | Miguel Raposo |
| Critical evaluation of results | Miguel Raposo |
| Real-hardware observations and final approval | Miguel Raposo |
| Calculations and repetitive technical analysis | ChatGPT under Miguel Raposo's direction |
| Code drafting and modification | Miguel Raposo with substantial ChatGPT assistance |
| Refactoring and debugging support | ChatGPT under Miguel Raposo's direction and validation |
| Scripts, build helpers and automation | ChatGPT under Miguel Raposo's direction |
| Repetitive repository and release work | ChatGPT under Miguel Raposo's direction |
| Technical documentation | Miguel Raposo with drafting and structuring assistance from ChatGPT |

## Why the workflow is structured this way

Long-running AI-assisted development has specific failure modes. A model can lose context between sessions, make incorrect assumptions, overgeneralize from incomplete evidence, propose technically plausible but unwanted changes, or produce code that still requires real validation. It also cannot independently inspect the physical target device or preserve a trustworthy project state merely through conversation history.

The workflow is deliberately designed around those limitations rather than pretending they do not exist.

### 1. Human decisions are explicit checkpoints

Important design, scope and quality decisions are not delegated implicitly. Miguel Raposo can approve, reject or redirect an implementation after reviewing the result. A technically possible change is not automatically a desirable project decision.

This distinction has repeatedly mattered in practice. Experiments can be technically successful and still be rejected because they do not meet the intended visual, interaction or architectural standard.

### 2. The project state lives outside the chat

Conversation memory is not treated as the authoritative project database. Durable state is externalized into source control, documentation, handovers, evidence files, release tags and checksums where appropriate.

This makes the project recoverable even when a conversation ends, context is reduced, or a later session starts without the full reasoning history.

### 3. Handovers are a continuity mechanism

Versioned handovers record the current baseline, what has passed, what has failed, what remains open, the relevant evidence and the exact next step.

The handover is not merely a summary. It is a control mechanism for preventing an AI assistant from silently reconstructing the project from incomplete memory or restarting work from an obsolete assumption.

A useful handover separates:

- confirmed facts from hypotheses;
- approved decisions from experiments;
- current baseline from rejected branches;
- completed gates from pending gates;
- durable project files from temporary evidence or tooling;
- the next required action from optional future ideas.

### 4. Evidence is stronger than conversational confidence

A statement such as "the build succeeded" is not accepted merely because a command returned zero or because ChatGPT predicts that it should work.

Where possible, the workflow asks for evidence that directly proves the relevant condition: expected artifacts, hashes, dependency commits, semantic APK checks, logs, screenshots, clean-clone results and real-device observations.

Machine evidence and human QA are kept distinct. A script can prove that a file exists or that an APK contains the expected architecture. It cannot decide whether a control scheme feels correct, whether an image is visually faithful, or whether an audio result is acceptable on the physical device.

### 5. Real hardware remains an independent authority

The Retroid Pocket 5 is not treated as an abstract Android target. Hardware-specific behavior is tested on the device and Miguel Raposo reports and evaluates the observed result.

ChatGPT can help interpret logs, formulate tests, calculate values and propose fixes, but it cannot replace the human operator who sees, hears and controls the actual device.

### 6. Interaction is deliberately incremental

For risky or stateful operations, work is often performed one confirmed step at a time. This reduces the cost of a wrong assumption and gives Miguel Raposo the opportunity to inspect intermediate results before the next mutation.

This is especially useful when dealing with builds, device installation, file movement, release operations, repository changes or experiments that may affect an approved baseline.

### 7. Repetition is automated, judgment is not

Automation is used aggressively for work that is deterministic, repetitive or easy to verify: calculations, file generation, dependency bootstrap, build commands, checks, repository maintenance, formatting and similar grind work.

The purpose is to save human time for the parts that need intent and judgment: deciding what the project should become, identifying when an apparently reasonable solution is wrong for the design, choosing between trade-offs, spotting unwanted creative assumptions and approving the final result.

### 8. Failed approaches remain useful evidence

Rejected experiments and AI mistakes are not automatically erased from the engineering history. When they reveal a root cause, a limitation or a reusable rule, they are recorded in the knowledge base.

This is particularly important with AI-assisted development because the same plausible but incorrect approach can otherwise be proposed again in a later session.

## Project organization and recovery

The working method separates durable project state from disposable working material.

Durable source, documentation and project decisions belong in the controlled project tree and public repository where redistribution is appropriate. Commercial game data, private material, caches, temporary build products and disposable evidence remain outside redistributable source.

Recovery and backup do not depend on a single mechanism. The project uses complementary layers such as Git history, release tags, versioned handovers, evidence bundles and checksums. These make it possible to identify an approved baseline, reproduce a public snapshot and recover the reasoning context required to continue development.

The exact local development environment may contain additional private evidence and handover material that is intentionally not part of the public source release.

## Working with ChatGPT's limitations

The workflow assumes that ChatGPT can be useful and still be wrong.

Common risks include:

- loss of context across long or separate sessions;
- confident but incorrect technical assumptions;
- changing something that was not requested;
- treating an experiment as an approved design decision;
- confusing a process success with artifact or runtime success;
- generating excessive work where a smaller controlled change is safer;
- overlooking constraints that are obvious only from physical-device use;
- repeating a previously rejected idea after the relevant context has fallen out of the active conversation.

The countermeasures are practical rather than rhetorical: explicit constraints, one-step checkpoints, versioned handovers, external documentation, exact baselines, hashes, clean-clone tests, real-hardware QA, preserved negative results and direct human correction when the assistant drifts from the intended design.

Miguel Raposo's critical and creative role is therefore not an optional review layer added after AI output. It is the mechanism that gives the work direction and keeps the automation aligned with the actual project.

## What ChatGPT contributes

ChatGPT is valuable here because it can perform large amounts of implementation and grind work quickly. It can explore code, draft changes, calculate values, prepare scripts, compare evidence, restructure documentation, automate repetitive actions and help reason through technical failures.

That assistance can substantially reduce development time.

However, ChatGPT cannot autonomously carry this specific project from concept to validated result. It does not possess Miguel Raposo's project intent, creative priorities, accumulated design choices, physical-device observations or authority to decide what counts as correct. Without his direction, constraints, evidence, critical review and approvals, the project would not exist in this form and the assistant could not independently continue it as the same project.

## Attribution principle

The concise attribution used throughout the project is:

> Human-directed, AI-assisted. Miguel Raposo owns the design direction, decisions, critical evaluation and final approval. ChatGPT provides substantial assistance with calculations, programming, automation, analysis, documentation and repetitive technical work.
