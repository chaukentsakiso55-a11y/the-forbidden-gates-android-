# The Forbidden Gates Android — Campaign Status Through Level 100

The Android game now shares the complete canonical campaign progression with desktop through **Level 100 — What Lies Beyond**.

## Coverage

- Levels 1–3 use dedicated authored code prototype directors/world builders.
- Levels 4–100 use the reusable `TFGCampaignLevelRuntime` and `TFGCampaignCatalog`.
- The catalog contains every canonical title and premise from Level 4 through Level 100.
- GameMode automatically loads the correct runtime definition from persistent `CurrentLevel` save state.
- Older saves parked at the previous Level 50 midpoint hold automatically migrate into Level 51.

Each runtime level includes persistent objectives, dialogue/story interactions, combat encounters, elite encounters where appropriate, start/midpoint checkpoints, realm-pattern prototype geometry and level progression.

## Chapters implemented as playable code prototypes

- Chapter I — The Fall of Elaris: 1–10
- Chapter II — The Whispering Wilds: 11–20
- Chapter III — The Ember Kingdom: 21–30
- Chapter IV — The Sunken Realm: 31–40
- Chapter V — The Stormlands: 41–50
- Chapter VI — The Broken Kingdom: 51–60
- Chapter VII — The Shadow Realm: 61–70
- Chapter VIII — The Ancient World: 71–80
- Chapter IX — The Last Gate: 81–90
- Chapter X — The Heart Gate: 91–100

## Major back-half story systems

The runtime includes dedicated story beats for the King's secret, Vael the Fallen, Kael's Heart Gate failsafe lineage, Orion and the complete Hollow history, Emperor Vaelor, the return to Elaris, Aldren's confrontation, the Heart Gate, and the First Hollow.

Persistent campaign rewards include the recovered Gate Sigil, King's records, Umbral traversal, Shadow Seal, Ardyn lineage record, Elyra's controlled Gatefire pattern, Heart Key, Last Sigil, final sealing ritual and Kael's Heart Gate Master bond.

## Level 100 finale

Level 100 is a true terminal campaign state rather than a transition into Level 101. After the First Hollow has been defeated in Level 99, the player reaches the restored Heart Gate and chooses one of three persistent endings:

1. **Seal the Gates**
2. **Destroy the Gates**
3. **Control the Gates**

The selected ending is stored in `NarrativeChoices` as `FinalGateFate`, Level 100 is marked complete, `CampaignComplete` is saved, and New Game+ is unlocked. Reopening a completed campaign returns to the Level 100 runtime instead of an empty next level.

## Validation

Android repository-health validation checks that every runtime level from 4–100 is present, the Level 50 Elyra midpoint exists, Level 100 is marked as the finale, all three ending choices exist, the old Level 50 save migration exists, and GameMode routes campaign levels through `ATFGCampaignLevelRuntime`.

Repository validation is not a full Unreal compile/cook/package/device test. Levels 4–100 currently use code-built prototype geometry and reusable gameplay actors. Final production art, animations, cinematic sequences, voice/audio, bespoke boss phases, puzzles, VFX, navigation tuning and Android performance QA remain separate production work.
