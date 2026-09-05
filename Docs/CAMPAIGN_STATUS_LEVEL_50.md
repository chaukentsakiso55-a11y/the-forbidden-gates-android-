# The Forbidden Gates Android — Campaign Status Through Level 50

The Android game now shares the canonical campaign progression with desktop through **Level 50 — Elyra**.

## Coverage

- Levels 1–3 use their dedicated authored code prototype directors/world builders.
- Levels 4–50 use the reusable `TFGCampaignLevelRuntime` and `TFGCampaignCatalog`.
- The catalog contains every canonical title and premise from Level 4 through Level 50.
- GameMode automatically loads the correct runtime definition from persistent `CurrentLevel` save state.

Each runtime level includes persistent objectives, dialogue/story interactions, combat encounters, elite encounters where appropriate, start/midpoint checkpoints, realm-pattern prototype geometry and a level-completion transition.

## Chapters implemented as playable prototypes

- Chapter I — The Fall of Elaris: 1–10
- Chapter II — The Whispering Wilds: 11–20
- Chapter III — The Ember Kingdom: 21–30
- Chapter IV — The Sunken Realm: 31–40
- Chapter V — The Stormlands: 41–50

Persistent campaign rewards include Gate Sigils/keys, Verdant progression, Ember relics, Tide records/mastery, Storm records and Dominion evidence. Quest-linked relic recovery is idempotent: a recovery interaction can repair quest progress without duplicating inventory or mastery rewards.

## Level 50 midpoint behavior

Level 50 deliberately does not load Level 51. Kael reaches Elyra, hears that she will not return to Elaris, learns that the seals are failing, and completes the midpoint state while remaining in the Level 50 runtime scene.

## Validation

Android repository-health validation now checks that all Levels 4–50 are present in the runtime catalog, that the Elyra midpoint exists and that GameMode routes runtime levels through `ATFGCampaignLevelRuntime`.

Repository validation is not a full Unreal compile/cook/package/device test. Levels 4–50 currently use code-built prototype geometry and reusable runtime gameplay actors. Final production art, animations, cinematics, VFX, bespoke bosses/puzzles, audio and device-performance QA remain separate production work.
