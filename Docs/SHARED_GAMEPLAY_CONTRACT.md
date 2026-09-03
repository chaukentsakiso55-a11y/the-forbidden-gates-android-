# Shared gameplay contract

The Windows and Android editions use the same `TheForbiddenGates` Unreal module and gameplay class names. Platform differences belong in configuration, device profiles, UI layout, rendering budgets and platform integrations—not in story or progression rules.

## Must stay identical across platforms
- 100-level campaign order and required objectives
- save schema/version migration
- quest identifiers and stages
- Gate and relic identifiers
- ability identifiers and unlock rules
- narrative choices and ending requirements
- core player/enemy attributes

## Android-only concerns
- touch bindings and configurable control placement
- safe areas and scalable UI
- device profiles and thermal fallback
- texture/mesh/VFX LOD budgets
- Android lifecycle save resilience
- AAB packaging and signing

When shared runtime code changes in one repository, mirror it to the other before a release candidate is cut.
