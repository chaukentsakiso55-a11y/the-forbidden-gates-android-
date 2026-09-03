# Android Production Plan

## Baseline
- Unreal Engine: 5.8.2
- Store target: Android 16 / API 36
- Install baseline: Android 8 / API 26 where device capability is sufficient
- CPU: 64-bit ARM
- Graphics: Vulkan preferred on supported devices; fallback rendering profile where required
- Distribution: Android App Bundle for release, APK for local QA builds

## Shared-content contract
The desktop repository owns canonical story, quest IDs, level IDs, character names, magic disciplines, narrative choices and save schema. Android owns mobile rendering profiles, touch input, mobile UI layouts, packaging configuration and mobile performance budgets.

Any gameplay feature added to desktop must be evaluated for Android at design time. Mobile optimization may change presentation cost, but not required objectives or story information.

## Quality tiers
### Low
- 30 fps target
- reduced view distance
- reduced foliage density
- simplified shadows and post processing
- lower particle counts
- lower texture streaming budgets

### Medium
- 30 fps target with increased scene density and effects
- improved shadows and material quality

### High
- 45/60 fps option only after sustained thermal testing
- higher texture, VFX and view-distance budgets

## Touch controls
- left movement region
- right camera region
- context-sensitive interaction control
- magic ability tray
- evade/traversal action
- target-assist toggle where appropriate
- pause/menu access
- fully adjustable scale and opacity

All touch actions must map to the same Enhanced Input action names used by desktop controller/keyboard inputs.

## Mobile UX
- large readable touch targets
- safe-area support
- scalable text
- subtitle controls
- no critical information hidden by device cutouts
- autosave before app suspension-sensitive sequences
- graceful recovery after OS backgrounding

## Memory strategy
- stream level sections instead of loading full long levels at once
- aggressive texture mip streaming
- platform-specific LODs
- pooled frequently spawned VFX objects where beneficial
- cap simultaneous high-cost magic effects
- remove unused editor-only assets from shipping packages

## Thermal strategy
Sustained play sessions matter more than short benchmark peaks. Higher frame-rate modes must fall back safely when device temperature or frame pacing becomes unstable.

## QA device matrix
Test at least:
1. baseline supported ARM64 Android device;
2. mid-range device;
3. high-end device;
4. different GPU families where available;
5. controller-connected device;
6. devices with display cutouts and gesture navigation.

## Release validation
- clean install
- update from prior release
- save migration
- suspend/resume
- low-storage behavior
- offline launch
- controller hot-plug
- touch remapping persistence
- all 100 levels load and complete
- no blocker crashes
- package target reports API 36
- signed AAB upload validation
