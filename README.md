# UncapperMCM

SkyUI MCM for Skyrim Skill Uncapper SE/AE with per-save runtime overrides.

This project provides an in-game Mod Configuration Menu for controlling supported Skyrim Skill Uncapper settings without rewriting the global `SkyrimUncapper.ini`.

## Current Features

Implemented support includes:

- Enable / disable per-save runtime overrides
- Reset values back to the currently loaded `SkyrimUncapper.ini`
- Skill Caps
- Skill Formula Caps
- Enchanting settings
- Skill XP multipliers
- Skill XP breakpoints by base skill level
- Skill XP breakpoints by character level
- Player Level XP multipliers
- Player Level XP breakpoints
- Bulk editing with `All Skills`

Additional features are still in development.

## Architecture

UncapperMCM is composed of:

- a C++ SKSE plugin;
- Papyrus scripts;
- a SkyUI MCM;
- SKSE serialization for per-save settings.

Runtime overrides are applied through a modified Skyrim Skill Uncapper runtime:

https://github.com/RobinL34/SkyrimUncapper-MCM-Runtime

`SkyrimUncapper.ini` remains the baseline configuration.

When runtime overrides are disabled or reset, Skyrim Skill Uncapper uses the values loaded from the INI.

## Requirements

- Skyrim Special Edition / Anniversary Edition
- SKSE
- SkyUI
- Skyrim Skill Uncapper SE/AE
- SkyrimUncapper MCM Runtime

Development is currently targeting Skyrim AE 1.6.1170.

## Status

Work in progress.

The mod is currently under active development and should not yet be considered a finished release.

## Credits

### Skyrim Skill Uncapper

UncapperMCM is designed to work with Skyrim Skill Uncapper SE/AE.

Original and previous implementations include:

- Andrew Spaulding / TheDreadedAndy
- Kassent
- Vadfromnu
- Elys

### UncapperMCM

MCM integration, runtime override system and per-save configuration:

- Robin

## License

License information for UncapperMCM will be finalized before release.

The modified Skyrim Skill Uncapper runtime is maintained separately and retains the licensing requirements of the original project.