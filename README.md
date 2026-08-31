# UncapperMCM

SkyUI MCM for **Skyrim Skill Uncapper SE/AE** with per-save runtime overrides.

UncapperMCM provides an in-game configuration interface for supported Skyrim Skill Uncapper settings without rewriting the global `SkyrimUncapper.ini`.

Settings are stored per save through SKSE serialization and applied at runtime through the companion SkyrimUncapper MCM Runtime.

## Features

Current support includes:

- Enable / disable per-save runtime overrides
- Reset all runtime overrides back to the currently loaded `SkyrimUncapper.ini`
- Skill Caps
- Skill Formula Caps
- Enchanting settings
- Skill XP base and offset multipliers
- Skill XP breakpoints by skill level
- Skill XP breakpoints by character level
- Player Level XP multipliers
- Player Level XP breakpoints by skill level
- Player Level XP breakpoints by character level
- Perks at Level Up
- Attributes at Level Up
- Legendary Skill settings
- Bulk editing with `All Skills`
- Per-save configuration through SKSE serialization

## Runtime Behavior

`SkyrimUncapper.ini` remains the baseline configuration.

When UncapperMCM is disabled:

- runtime overrides are cleared;
- Skyrim Skill Uncapper returns to the values loaded from `SkyrimUncapper.ini`.

When UncapperMCM is enabled:

- the settings stored in the current save are applied as runtime overrides.

Using **Reset to INI** clears the runtime overrides, reloads the current INI baseline and disables the MCM runtime layer.

UncapperMCM never writes to `SkyrimUncapper.ini`.

## Architecture

UncapperMCM is composed of:

- a C++ SKSE plugin;
- Papyrus native bindings;
- a SkyUI MCM;
- SKSE serialization for per-save settings.

Runtime overrides are applied through the companion modified Skyrim Skill Uncapper runtime:

https://github.com/RobinL34/SkyrimUncapper-MCM-Runtime

Communication between both plugins uses a small C ABI resolved dynamically from `SkyrimUncapper.dll`.

## Per-Save Settings

UncapperMCM stores its configuration inside the Skyrim save through the SKSE serialization interface.

The current serialization format is **V7**.

Older supported save formats are migrated automatically when loaded.

Settings introduced in newer versions inherit the currently loaded `SkyrimUncapper.ini` baseline when loading an older UncapperMCM save.

Static startup options are not serialized.

## Static INI Settings

Some Skyrim Skill Uncapper features install their hooks only when Skyrim starts.

These options therefore remain controlled by `SkyrimUncapper.ini` and cannot be enabled dynamically from the MCM.

This currently includes settings such as:

- `bUseAttributesAtLevelUp`
- `bUseLegendarySettings`

When the corresponding startup hook is disabled, the related MCM controls are unavailable.

A full Skyrim restart is required after changing these startup options in the INI.

## Legendary Skills

The MCM supports runtime configuration for:

- Legendary skill level threshold
- Skill level after becoming Legendary
- Keep current skill level
- Hide Legendary button

`bUseLegendarySettings` itself remains a startup-only INI option.

The original Skyrim Skill Uncapper compatibility limitations for Legendary functionality remain unchanged.

## Requirements

- Skyrim Special Edition / Anniversary Edition
- SKSE
- SkyUI
- Skyrim Skill Uncapper MCM Runtime

Development and testing currently target:

**Skyrim AE 1.6.1170**

The original Skyrim Skill Uncapper installation or its required configuration files must also be present unless the companion runtime package is distributed as a complete replacement.

## Compatibility

UncapperMCM is designed specifically for the matching version of:

**SkyrimUncapper MCM Runtime**

Using an incompatible or unmodified `SkyrimUncapper.dll` will prevent some or all runtime functionality from working.

The plugin assumes runtime configuration mutations are serialized through its normal SKSE/Papyrus execution path. It does not expose the runtime API as a general multi-writer interface.

## Credits

### Skyrim Skill Uncapper

UncapperMCM is built to extend **Skyrim Skill Uncapper SE/AE**.

Original Rust implementation:

- Andrew Spaulding / TheDreadedAndy

Previous Skyrim Uncapper implementations:

- Kassent — Skyrim Uncapper SE
- Vadfromnu — SE/AE update of Kassent's implementation
- Elys — original Skyrim LE Uncapper

### UncapperMCM

MCM integration, C++ SKSE bridge, per-save serialization and runtime override integration:

- Robin

## Related Project

SkyrimUncapper MCM Runtime:

https://github.com/RobinL34/SkyrimUncapper-MCM-Runtime

Original Skyrim Skill Uncapper SE/AE:

https://www.nexusmods.com/skyrimspecialedition/mods/82558

Original repository:

https://github.com/TheDreadedAndy/SkyrimAEUncapper

## License

UncapperMCM is distributed separately from the modified Skyrim Skill Uncapper runtime.

The companion SkyrimUncapper MCM Runtime contains modified code from Skyrim Skill Uncapper SE/AE and retains the licensing requirements of the original project.

See the license files included with each repository for their respective terms.