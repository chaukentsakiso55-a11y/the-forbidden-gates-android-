# The Forbidden Gates — Android

Mobile production target for **The Forbidden Gates**, built with Unreal Engine 5.8.2 and designed to preserve the full story, progression and gameplay structure of the desktop edition while using mobile-specific rendering, input and memory profiles.

## Canonical scope
- 100 story levels across 10 chapters
- Same core story, endings, quests, Gate Masters and magic disciplines as desktop
- Third-person touch controls with optional controller support
- Device-profile quality tiers
- Checkpointed long-form levels
- Offline single-player core
- Android 8+ install baseline where supported by the engine/device profile
- Google Play production target: Android 16 / API 36

## Production rule
The desktop repository is the canonical narrative and gameplay-design source. Mobile may reduce visual cost, density, particles, texture resolution and background simulation, but must not remove required story beats, puzzle clues or progression mechanics.

## Performance targets
- Baseline supported devices: stable 30 fps target
- Mid/high devices: optional higher frame-rate modes after thermal testing
- Adaptive quality presets must prioritize readability and input responsiveness

## Android-specific systems
- Touch HUD and configurable control placement
- Safe-area handling for cutouts and gesture navigation
- Scalable UI and text
- Mobile pause/resume lifecycle handling
- Save integrity across app suspension
- Thermal/memory aware quality policy
- AAB release packaging for store distribution

## Toolchain baseline
Use the Android toolchain verified by Unreal Engine 5.8.2 through Turnkey. Keep the project target at API 36 for current Google Play submission requirements.

## Status
Mobile production foundation initialized. Final APK/AAB packages require Unreal Editor packaging, signing, on-device profiling, QA and release credentials.
