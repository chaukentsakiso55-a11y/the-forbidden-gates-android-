# The Forbidden Gates — Content Production Pipeline

This phase turns the code-complete 100-level prototype campaign into a finished game without changing campaign numbering or save compatibility.

## Production rule

Gameplay code, campaign state and final assets are separate layers. A level may be code-playable before final art exists, but it is not release-ready until its production checklist passes.

## Per-level asset contract

`TFGLevelContentProfile` assigns every level:

- environment theme
- lighting mood
- weather state
- music state
- traversal mechanic
- puzzle archetype
- optional intro/outro cinematic IDs
- optional named boss ID
- canonical art, audio and VFX content folders

Expected Unreal content roots:

- `/Game/Production/Worlds/Chapter_XX/Level_XXX`
- `/Game/Production/Audio/Chapter_XX/Level_XXX`
- `/Game/Production/VFX/Chapter_XX/Level_XXX`

Final assets can replace code-generated placeholder geometry without changing quest IDs, map IDs, save data or level numbering.

## Level completion gates

A production level is complete only when all applicable gates pass:

1. **Gameplay** — critical path, optional exploration, encounters, checkpoints and fail/retry behavior work.
2. **Environment** — final modular kit, landmarks, collision, traversal readability and level dressing are authored.
3. **Lighting** — level mood, exposure, readability and performance targets are validated on target hardware.
4. **Characters** — required hero/NPC/enemy models, rigs, animation sets and materials are final or release-approved.
5. **VFX** — magic, Gate effects, hazards, impacts and boss phase visuals have performance tiers.
6. **Audio** — music state, ambience, interaction sounds, combat audio and dialogue/voice requirements are hooked up.
7. **Cinematics** — required story sequences are authored, skippable where appropriate, subtitle-safe and replayable when the design calls for it.
8. **Accessibility** — subtitles, readable objective presentation, input parity and effects readability are checked.
9. **Performance** — CPU/GPU/memory/thermal budgets pass on Windows target specs and Android quality tiers.
10. **QA** — clean-start, checkpoint restore, save migration, death/retry, level transition and campaign progression tests pass.

## Hero levels

Hero levels receive bespoke art/cinematic attention before ordinary levels:

- 1 — opening Elaris
- 4 — Elyra vanishes
- 10 — Dominion Commander / First Gate
- 19 — Guardian of Roots
- 20 — Verdant Seal
- 29 — Seraphon
- 39 — Drowned King
- 40 — Ocean Gate
- 50 — Elyra midpoint reveal
- 59 — King's Secret
- 69 — Vael the Fallen
- 74 — Kael Ardyn reveal
- 77 — Orion
- 83 — collapsing Sky Route
- 84 — Emperor Vaelor
- 90 — return to Elaris
- 94 — Aldren confrontation
- 98 — Heart Gate entry
- 99 — First Hollow
- 100 — final Gate choice and consequences

## Boss production contract

Named bosses are currently represented by `ATFGCampaignEliteEnemy` with campaign-level boss profiles and three runtime health phases. Boss Blueprints should bind to `OnBossPhaseChanged` to drive:

- phase-specific animations
- ability loadouts
- arena changes
- music intensity
- lighting/VFX shifts
- dialogue barks
- camera emphasis

Named production bosses currently defined:

- Level 10 — Dominion Commander
- Level 19 — Guardian of Roots
- Level 29 — Seraphon, Master of Flame
- Level 39 — The Drowned King
- Level 69 — Vael the Fallen
- Level 99 — The First Hollow

## Mobile production rule

Android must preserve the same campaign and mechanics while using scalable content:

- lower LOD and texture tiers
- reduced particle counts
- simplified dynamic shadows where needed
- compressed audio tiers
- shorter streaming distances
- thermal fallback without removing required gameplay information

No mobile optimization may alter story choices, quest IDs, reward IDs or save compatibility.

## Current next target

Build the first final-quality vertical slice around Levels 1–10, with Level 10 as the first production boss benchmark. Once that slice passes the release gates, the same asset workflow scales chapter by chapter through Level 100.
