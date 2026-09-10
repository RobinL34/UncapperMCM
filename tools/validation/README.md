# UncapperMCM refactor validation

These scripts protect the stable Nexus `v1.0.0` contracts while the C++ source
is split into smaller translation units. They do not replace in-game testing.

Run the complete Phase 0 validation from the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File tools/validation/Validate-Refactor.ps1
```

The validator checks the Release DLL and its stable export names, plugin/DLL
naming, ordered Papyrus registrations, Papyrus declarations and signatures,
non-tasklet registration behavior, SkyrimUncapper ABI symbol names, the static
serialization contract and V1-V7 loaders, unique `g_settings` storage, protected
Phase 1-3 files, and `git diff --check`.

For later Settings phases, the temporary sensitive-file gate can be disabled
with `-SkipSensitiveFiles`; every other contract remains enforced.

Baseline capture is deliberately guarded. It only accepts `v1.0.0`, requires an
explicit confirmation switch and refuses to overwrite any existing snapshot:

```powershell
powershell -ExecutionPolicy Bypass -File tools/validation/Capture-Baseline.ps1 `
  -Tag v1.0.0 -BaselineDll build/Release/UncapperMCM.dll -ConfirmCreate
```

The supplied DLL must have been built from the tag with the approved Release
toolchain. Source snapshots are always read directly from the tag with
`git show`; the script never checks out or rewrites the working tree. `dumpbin`
is located through PATH or Visual Studio Installer. A native PowerShell PE
parser is used when `dumpbin` is unavailable.

Manual validation still required includes Skyrim AE 1.6.1170 startup, SKSE and
Papyrus logs, MCM navigation, Enabled ON/OFF, Reset to INI, save/load/restart,
V1-V7 migration fixtures, and every supported settings domain.

