ScriptName UncapperMCMConfig extends SKI_ConfigBase

Int OPTION_ENABLED
Int OPTION_RESET_TO_INI

Int OPTION_ENCHANT_MAGNITUDE_CAP
Int OPTION_ENCHANT_CHARGE_CAP
Int OPTION_ENCHANT_LINEAR_CHARGE

Int[] OPTION_SKILL_CAPS
Int[] OPTION_FORMULA_CAPS

String[] SKILL_NAMES
String[] SKILL_SELECTOR_NAMES


; ============================================================================
; Skill XP
; ============================================================================

Int SELECTED_XP_SKILL = 0
Bool XP_ALL_SKILLS = false

Int OPTION_XP_SKILL_SELECTOR
Int OPTION_XP_BASE_MULTIPLIER
Int OPTION_XP_OFFSET_MULTIPLIER
Int OPTION_XP_APPLY_ALL_BASE
Int OPTION_XP_APPLY_ALL_BREAKPOINTS

Int OPTION_XP_NEW_BREAKPOINT_LEVEL
Int OPTION_XP_ADD_BREAKPOINT
Int OPTION_XP_REMOVE_BREAKPOINT

Int[] OPTION_XP_BREAKPOINT_LEVELS
Int[] OPTION_XP_BREAKPOINT_BASES
Int[] OPTION_XP_BREAKPOINT_OFFSETS

Int ACTIVE_XP_BREAKPOINT_COUNT = 0

Bool CURRENT_XP_CHARACTER_LEVEL = false

Int NEW_XP_BREAKPOINT_LEVEL = 1

Int BULK_XP_BASE_MULTIPLIER = 100
Int BULK_XP_OFFSET_MULTIPLIER = 100

Int BULK_XP_SKILL_COUNT = 1
Int[] BULK_XP_SKILL_LEVELS
Int[] BULK_XP_SKILL_BASES
Int[] BULK_XP_SKILL_OFFSETS

Int BULK_XP_CHARACTER_COUNT = 1
Int[] BULK_XP_CHARACTER_LEVELS
Int[] BULK_XP_CHARACTER_BASES
Int[] BULK_XP_CHARACTER_OFFSETS


; ============================================================================
; Player Level XP
; ============================================================================

Int SELECTED_PLAYER_XP_SKILL = 0
Bool PLAYER_XP_ALL_SKILLS = false

Int OPTION_PLAYER_XP_SKILL_SELECTOR
Int OPTION_PLAYER_XP_MULTIPLIER
Int OPTION_PLAYER_XP_APPLY_ALL_BASE
Int OPTION_PLAYER_XP_APPLY_ALL_BREAKPOINTS

Int OPTION_PLAYER_XP_NEW_BREAKPOINT_LEVEL
Int OPTION_PLAYER_XP_ADD_BREAKPOINT
Int OPTION_PLAYER_XP_REMOVE_BREAKPOINT

Int[] OPTION_PLAYER_XP_BREAKPOINT_LEVELS
Int[] OPTION_PLAYER_XP_BREAKPOINT_MULTIPLIERS

Int ACTIVE_PLAYER_XP_BREAKPOINT_COUNT = 0

Bool CURRENT_PLAYER_XP_CHARACTER_LEVEL = false

Int NEW_PLAYER_XP_BREAKPOINT_LEVEL = 1

Int BULK_PLAYER_XP_MULTIPLIER = 100

Int BULK_PLAYER_XP_SKILL_COUNT = 1
Int[] BULK_PLAYER_XP_SKILL_LEVELS
Int[] BULK_PLAYER_XP_SKILL_MULTIPLIERS

Int BULK_PLAYER_XP_CHARACTER_COUNT = 1
Int[] BULK_PLAYER_XP_CHARACTER_LEVELS
Int[] BULK_PLAYER_XP_CHARACTER_MULTIPLIERS


; ============================================================================
; Perks at Level Up
; ============================================================================

Int OPTION_PERKS_NEW_BREAKPOINT_LEVEL
Int OPTION_PERKS_ADD_BREAKPOINT
Int OPTION_PERKS_REMOVE_BREAKPOINT

Int[] OPTION_PERKS_BREAKPOINT_LEVELS
Int[] OPTION_PERKS_BREAKPOINT_VALUES

Int ACTIVE_PERKS_BREAKPOINT_COUNT = 0
Int NEW_PERKS_BREAKPOINT_LEVEL = 1


; ============================================================================
; Attributes at Level Up
; ============================================================================

String[] ATTRIBUTE_CHOICE_NAMES
String[] ATTRIBUTE_TARGET_NAMES

Int SELECTED_ATTRIBUTE_CHOICE = 0
Int SELECTED_ATTRIBUTE_TARGET = 0

Int OPTION_ATTRIBUTE_CHOICE_SELECTOR
Int OPTION_ATTRIBUTE_TARGET_SELECTOR

Int OPTION_ATTRIBUTE_NEW_BREAKPOINT_LEVEL
Int OPTION_ATTRIBUTE_ADD_BREAKPOINT
Int OPTION_ATTRIBUTE_REMOVE_BREAKPOINT

Int[] OPTION_ATTRIBUTE_BREAKPOINT_LEVELS
Int[] OPTION_ATTRIBUTE_BREAKPOINT_VALUES

Int ACTIVE_ATTRIBUTE_BREAKPOINT_COUNT = 0
Int NEW_ATTRIBUTE_BREAKPOINT_LEVEL = 1


; ============================================================================
; Legendary Settings
; ============================================================================

Int OPTION_LEGENDARY_HOOK_STATUS
Int OPTION_LEGENDARY_SKILL_LEVEL
Int OPTION_LEGENDARY_AFTER_LEVEL
Int OPTION_LEGENDARY_KEEP_LEVEL
Int OPTION_LEGENDARY_HIDE_BUTTON
Int OPTION_LEGENDARY_HIGHEST_SKILL_CAP


String CURRENT_PAGE = ""


; ============================================================================
; Version
; ============================================================================

Int Function GetVersion()

    Return 12

EndFunction


; ============================================================================
; Initialization
; ============================================================================

Function InitializeMenuData()

    SKILL_NAMES = new String[18]

    SKILL_NAMES[0] = "One-Handed"
    SKILL_NAMES[1] = "Two-Handed"
    SKILL_NAMES[2] = "Archery"
    SKILL_NAMES[3] = "Block"
    SKILL_NAMES[4] = "Smithing"
    SKILL_NAMES[5] = "Heavy Armor"
    SKILL_NAMES[6] = "Light Armor"
    SKILL_NAMES[7] = "Pickpocket"
    SKILL_NAMES[8] = "Lockpicking"
    SKILL_NAMES[9] = "Sneak"
    SKILL_NAMES[10] = "Alchemy"
    SKILL_NAMES[11] = "Speech"
    SKILL_NAMES[12] = "Alteration"
    SKILL_NAMES[13] = "Conjuration"
    SKILL_NAMES[14] = "Destruction"
    SKILL_NAMES[15] = "Illusion"
    SKILL_NAMES[16] = "Restoration"
    SKILL_NAMES[17] = "Enchanting"

    SKILL_SELECTOR_NAMES = new String[19]
    SKILL_SELECTOR_NAMES[0] = "All Skills"

    Int skillMenuIndex = 0

    While skillMenuIndex < 18
        SKILL_SELECTOR_NAMES[skillMenuIndex + 1] = SKILL_NAMES[skillMenuIndex]
        skillMenuIndex += 1
    EndWhile

    OPTION_SKILL_CAPS = new Int[18]
    OPTION_FORMULA_CAPS = new Int[18]

    OPTION_XP_BREAKPOINT_LEVELS = new Int[32]
    OPTION_XP_BREAKPOINT_BASES = new Int[32]
    OPTION_XP_BREAKPOINT_OFFSETS = new Int[32]

    OPTION_PLAYER_XP_BREAKPOINT_LEVELS = new Int[32]
    OPTION_PLAYER_XP_BREAKPOINT_MULTIPLIERS = new Int[32]

    OPTION_PERKS_BREAKPOINT_LEVELS = new Int[32]
    OPTION_PERKS_BREAKPOINT_VALUES = new Int[32]

    ATTRIBUTE_CHOICE_NAMES = new String[3]
    ATTRIBUTE_CHOICE_NAMES[0] = "Health"
    ATTRIBUTE_CHOICE_NAMES[1] = "Magicka"
    ATTRIBUTE_CHOICE_NAMES[2] = "Stamina"

    ATTRIBUTE_TARGET_NAMES = new String[4]
    ATTRIBUTE_TARGET_NAMES[0] = "Health"
    ATTRIBUTE_TARGET_NAMES[1] = "Magicka"
    ATTRIBUTE_TARGET_NAMES[2] = "Stamina"
    ATTRIBUTE_TARGET_NAMES[3] = "Carry Weight"

    OPTION_ATTRIBUTE_BREAKPOINT_LEVELS = new Int[32]
    OPTION_ATTRIBUTE_BREAKPOINT_VALUES = new Int[32]

    BULK_XP_SKILL_LEVELS = new Int[32]
    BULK_XP_SKILL_BASES = new Int[32]
    BULK_XP_SKILL_OFFSETS = new Int[32]

    BULK_XP_CHARACTER_LEVELS = new Int[32]
    BULK_XP_CHARACTER_BASES = new Int[32]
    BULK_XP_CHARACTER_OFFSETS = new Int[32]

    BULK_PLAYER_XP_SKILL_LEVELS = new Int[32]
    BULK_PLAYER_XP_SKILL_MULTIPLIERS = new Int[32]

    BULK_PLAYER_XP_CHARACTER_LEVELS = new Int[32]
    BULK_PLAYER_XP_CHARACTER_MULTIPLIERS = new Int[32]

    BULK_XP_SKILL_LEVELS[0] = 0
    BULK_XP_SKILL_BASES[0] = 100
    BULK_XP_SKILL_OFFSETS[0] = 100

    BULK_XP_CHARACTER_LEVELS[0] = 0
    BULK_XP_CHARACTER_BASES[0] = 100
    BULK_XP_CHARACTER_OFFSETS[0] = 100

    BULK_PLAYER_XP_SKILL_LEVELS[0] = 0
    BULK_PLAYER_XP_SKILL_MULTIPLIERS[0] = 100

    BULK_PLAYER_XP_CHARACTER_LEVELS[0] = 0
    BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[0] = 100

    Pages = new String[12]

    Pages[0] = "General"
    Pages[1] = "Skill Caps"
    Pages[2] = "Formula Caps"
    Pages[3] = "Enchanting"
    Pages[4] = "Skill XP"
    Pages[5] = "XP by Skill Level"
    Pages[6] = "XP by Character Level"
    Pages[7] = "Player Level XP"
    Pages[8] = "Player XP by Skill Level"
    Pages[9] = "Player XP by Character Level"
    Pages[10] = "Perks at Level Up"
    Pages[11] = "Attributes at Level Up"

EndFunction


Event OnConfigInit()

    ModName = "Uncapper MCM"

    InitializeMenuData()

EndEvent


Event OnVersionUpdate(Int newVersion)

    If newVersion >= 12

        ModName = "Uncapper MCM"

        InitializeMenuData()

        Debug.Trace("[UncapperMCM] MCM updated to version " + newVersion)

    EndIf

EndEvent


; ============================================================================
; Page rendering
; ============================================================================

Event OnPageReset(String page)

    If page == ""
        page = "General"
    EndIf

    CURRENT_PAGE = page

    UnloadCustomContent()

    Bool enabled = UncapperMCM.GetEnabled()

    If page == "General"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Uncapper MCM")

        OPTION_ENABLED = AddToggleOption("Enabled", enabled)

        AddEmptyOption()

        OPTION_RESET_TO_INI = AddTextOption("Reset to INI", "Reset")

        AddEmptyOption()

        AddTextOption("Configuration", "Per-save overrides")

        AddEmptyOption()

        AddHeaderOption("Legendary Skills")

        Bool legendaryHookEnabled = UncapperMCM.GetIniUseLegendarySettings()

        If legendaryHookEnabled
            OPTION_LEGENDARY_HOOK_STATUS = AddTextOption("Legendary Hooks", "Enabled in INI")
        Else
            OPTION_LEGENDARY_HOOK_STATUS = AddTextOption("Legendary Hooks", "Disabled in INI")
        EndIf

        Int legendaryFlags = OPTION_FLAG_NONE

        If !enabled || !legendaryHookEnabled
            legendaryFlags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_LEGENDARY_SKILL_LEVEL = AddSliderOption("Legendary Skill Level", UncapperMCM.GetLegendarySkillLevel(), "{0}", legendaryFlags)

        OPTION_LEGENDARY_AFTER_LEVEL = AddSliderOption("Skill Level After Legendary", UncapperMCM.GetLegendarySkillLevelAfter(), "{0}", legendaryFlags)

        OPTION_LEGENDARY_KEEP_LEVEL = AddToggleOption("Keep Skill Level", UncapperMCM.GetLegendaryKeepSkillLevel(), legendaryFlags)

        OPTION_LEGENDARY_HIDE_BUTTON = AddToggleOption("Hide Legendary Button", UncapperMCM.GetLegendaryHideButton(), legendaryFlags)

        OPTION_LEGENDARY_HIGHEST_SKILL_CAP = AddTextOption("Highest Skill Cap", GetHighestSkillCap() as String)


    ElseIf page == "Skill Caps"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Skill Caps")

        Int flags = OPTION_FLAG_NONE

        If !enabled
            flags = OPTION_FLAG_DISABLED
        EndIf

        Int i = 0

        While i < 18

            OPTION_SKILL_CAPS[i] = AddSliderOption(SKILL_NAMES[i], UncapperMCM.GetSkillCap(i), "{0}", flags)

            i += 1

        EndWhile


    ElseIf page == "Formula Caps"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Formula Caps")

        Int flags = OPTION_FLAG_NONE

        If !enabled
            flags = OPTION_FLAG_DISABLED
        EndIf

        Int i = 0

        While i < 18

            OPTION_FORMULA_CAPS[i] = AddSliderOption(SKILL_NAMES[i], UncapperMCM.GetFormulaCap(i), "{0}", flags)

            i += 1

        EndWhile


    ElseIf page == "Enchanting"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Enchanting")

        Int flags = OPTION_FLAG_NONE

        If !enabled
            flags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_ENCHANT_MAGNITUDE_CAP = AddSliderOption("Magnitude Level Cap", UncapperMCM.GetEnchantMagnitudeCap(), "{0}", flags)

        OPTION_ENCHANT_CHARGE_CAP = AddSliderOption("Charge Level Cap", UncapperMCM.GetEnchantChargeCap(), "{0}", flags)

        OPTION_ENCHANT_LINEAR_CHARGE = AddToggleOption("Use Linear Charge Formula", UncapperMCM.GetEnchantUseLinearCharge(), flags)


    ElseIf page == "Skill XP"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Skill Experience")

        Int flags = OPTION_FLAG_NONE

        If !enabled
            flags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_XP_SKILL_SELECTOR = AddMenuOption("Skill", GetXpSelectionLabel(), flags)

        AddEmptyOption()

        AddHeaderOption("Base Multipliers")

        If XP_ALL_SKILLS

            OPTION_XP_BASE_MULTIPLIER = AddSliderOption("Base XP Multiplier", HundredthsToFloat(BULK_XP_BASE_MULTIPLIER), "x{2}", flags)

            OPTION_XP_OFFSET_MULTIPLIER = AddSliderOption("Offset XP Multiplier", HundredthsToFloat(BULK_XP_OFFSET_MULTIPLIER), "x{2}", flags)

            AddEmptyOption()

            OPTION_XP_APPLY_ALL_BASE = AddTextOption("Apply Multipliers to All Skills", "Apply", flags)

        Else

            OPTION_XP_BASE_MULTIPLIER = AddSliderOption("Base XP Multiplier", HundredthsToFloat(UncapperMCM.GetSkillExpBaseMultiplier(SELECTED_XP_SKILL)), "x{2}", flags)

            OPTION_XP_OFFSET_MULTIPLIER = AddSliderOption("Offset XP Multiplier", HundredthsToFloat(UncapperMCM.GetSkillExpOffsetMultiplier(SELECTED_XP_SKILL)), "x{2}", flags)

        EndIf

        AddEmptyOption()

        AddTextOption("Skill Level Breakpoints", "Use next page", flags)

        AddTextOption("Character Level Breakpoints", "Use next page", flags)


    ElseIf page == "XP by Skill Level"

        CURRENT_XP_CHARACTER_LEVEL = false

        DrawXpBreakpointPage(false, enabled)


    ElseIf page == "XP by Character Level"

        CURRENT_XP_CHARACTER_LEVEL = true

        DrawXpBreakpointPage(true, enabled)


    ElseIf page == "Player Level XP"

        SetCursorFillMode(TOP_TO_BOTTOM)

        AddHeaderOption("Player Level Experience")

        Int flags = OPTION_FLAG_NONE

        If !enabled
            flags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_PLAYER_XP_SKILL_SELECTOR = AddMenuOption("Skill", GetPlayerXpSelectionLabel(), flags)

        AddEmptyOption()

        AddHeaderOption("Leveling Multiplier")

        If PLAYER_XP_ALL_SKILLS

            OPTION_PLAYER_XP_MULTIPLIER = AddSliderOption("Player Level XP Multiplier", HundredthsToFloat(BULK_PLAYER_XP_MULTIPLIER), "x{2}", flags)

            AddEmptyOption()

            OPTION_PLAYER_XP_APPLY_ALL_BASE = AddTextOption("Apply Multiplier to All Skills", "Apply", flags)

        Else

            OPTION_PLAYER_XP_MULTIPLIER = AddSliderOption("Player Level XP Multiplier", HundredthsToFloat(UncapperMCM.GetPlayerLevelExpMultiplier(SELECTED_PLAYER_XP_SKILL)), "x{2}", flags)

        EndIf

        AddEmptyOption()

        AddTextOption("Skill Level Breakpoints", "Use next page", flags)

        AddTextOption("Character Level Breakpoints", "Use next page", flags)


    ElseIf page == "Player XP by Skill Level"

        CURRENT_PLAYER_XP_CHARACTER_LEVEL = false

        DrawPlayerXpBreakpointPage(false, enabled)


    ElseIf page == "Player XP by Character Level"

        CURRENT_PLAYER_XP_CHARACTER_LEVEL = true

        DrawPlayerXpBreakpointPage(true, enabled)


    ElseIf page == "Perks at Level Up"

        DrawPerksAtLevelUpPage(enabled)


    ElseIf page == "Attributes at Level Up"

        DrawAttributesAtLevelUpPage(enabled)

    EndIf

EndEvent


; ============================================================================
; Draw XP breakpoint page
; ============================================================================

Function DrawXpBreakpointPage(Bool characterLevel, Bool enabled)

    SetCursorFillMode(TOP_TO_BOTTOM)

    Int flags = OPTION_FLAG_NONE

    If !enabled
        flags = OPTION_FLAG_DISABLED
    EndIf

    If characterLevel
        AddHeaderOption("XP by Character Level")
    Else
        AddHeaderOption("XP by Skill Level")
    EndIf

    OPTION_XP_SKILL_SELECTOR = AddMenuOption("Skill", GetXpSelectionLabel(), flags)

    AddEmptyOption()

    If characterLevel
        AddTextOption("Breakpoint Type", "Character Level", flags)
    Else
        AddTextOption("Breakpoint Type", "Base Skill Level", flags)
    EndIf

    AddEmptyOption()

    AddHeaderOption("Add / Remove Breakpoint")

    OPTION_XP_NEW_BREAKPOINT_LEVEL = AddSliderOption("New Breakpoint Level", NEW_XP_BREAKPOINT_LEVEL, "{0}", flags)

    OPTION_XP_ADD_BREAKPOINT = AddTextOption("Add Breakpoint", "Add", flags)

    Int count = 0

    If XP_ALL_SKILLS
        count = GetBulkXpCount(characterLevel)
    Else
        count = UncapperMCM.GetSkillExpBreakpointCount(SELECTED_XP_SKILL, characterLevel)
    EndIf

    ACTIVE_XP_BREAKPOINT_COUNT = count

    Int removeFlags = flags

    If count <= 1
        removeFlags = OPTION_FLAG_DISABLED
    EndIf

    OPTION_XP_REMOVE_BREAKPOINT = AddMenuOption("Remove Breakpoint", "Choose...", removeFlags)

    AddEmptyOption()

    AddHeaderOption("Breakpoints")

    Int i = 0

    While i < count

        Int level = 0
        Int baseMult = 100
        Int offsetMult = 100

        If XP_ALL_SKILLS
            level = GetBulkXpLevel(characterLevel, i)
            baseMult = GetBulkXpBase(characterLevel, i)
            offsetMult = GetBulkXpOffset(characterLevel, i)
        Else
            level = UncapperMCM.GetSkillExpBreakpointLevel(SELECTED_XP_SKILL, characterLevel, i)
            baseMult = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(SELECTED_XP_SKILL, characterLevel, i)
            offsetMult = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(SELECTED_XP_SKILL, characterLevel, i)
        EndIf

        Int levelFlags = flags

        If i == 0
            levelFlags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_XP_BREAKPOINT_LEVELS[i] = AddSliderOption("Breakpoint " + (i + 1) + " Level", level, "{0}", levelFlags)

        OPTION_XP_BREAKPOINT_BASES[i] = AddSliderOption("Base @ Level " + level, HundredthsToFloat(baseMult), "x{2}", flags)

        OPTION_XP_BREAKPOINT_OFFSETS[i] = AddSliderOption("Offset @ Level " + level, HundredthsToFloat(offsetMult), "x{2}", flags)

        i += 1

    EndWhile

    If XP_ALL_SKILLS
        AddEmptyOption()
        OPTION_XP_APPLY_ALL_BREAKPOINTS = AddTextOption("Apply Table to All Skills", "Apply", flags)
    EndIf

EndFunction


; ============================================================================
; Draw Player Level XP breakpoint page
; ============================================================================

Function DrawPlayerXpBreakpointPage(Bool characterLevel, Bool enabled)

    SetCursorFillMode(TOP_TO_BOTTOM)

    Int flags = OPTION_FLAG_NONE

    If !enabled
        flags = OPTION_FLAG_DISABLED
    EndIf

    If characterLevel
        AddHeaderOption("Player XP by Character Level")
    Else
        AddHeaderOption("Player XP by Skill Level")
    EndIf

    OPTION_PLAYER_XP_SKILL_SELECTOR = AddMenuOption("Skill", GetPlayerXpSelectionLabel(), flags)

    AddEmptyOption()

    If characterLevel
        AddTextOption("Breakpoint Type", "Character Level", flags)
    Else
        AddTextOption("Breakpoint Type", "Base Skill Level", flags)
    EndIf

    AddEmptyOption()

    AddHeaderOption("Add / Remove Breakpoint")

    OPTION_PLAYER_XP_NEW_BREAKPOINT_LEVEL = AddSliderOption("New Breakpoint Level", NEW_PLAYER_XP_BREAKPOINT_LEVEL, "{0}", flags)

    OPTION_PLAYER_XP_ADD_BREAKPOINT = AddTextOption("Add Breakpoint", "Add", flags)

    Int count = 0

    If PLAYER_XP_ALL_SKILLS
        count = GetBulkPlayerXpCount(characterLevel)
    Else
        count = UncapperMCM.GetPlayerLevelExpBreakpointCount(SELECTED_PLAYER_XP_SKILL, characterLevel)
    EndIf

    ACTIVE_PLAYER_XP_BREAKPOINT_COUNT = count

    Int removeFlags = flags

    If count <= 1
        removeFlags = OPTION_FLAG_DISABLED
    EndIf

    OPTION_PLAYER_XP_REMOVE_BREAKPOINT = AddMenuOption("Remove Breakpoint", "Choose...", removeFlags)

    AddEmptyOption()

    AddHeaderOption("Breakpoints")

    Int i = 0

    While i < count

        Int level = 0
        Int multiplier = 100

        If PLAYER_XP_ALL_SKILLS
            level = GetBulkPlayerXpLevel(characterLevel, i)
            multiplier = GetBulkPlayerXpMultiplier(characterLevel, i)
        Else
            level = UncapperMCM.GetPlayerLevelExpBreakpointLevel(SELECTED_PLAYER_XP_SKILL, characterLevel, i)
            multiplier = UncapperMCM.GetPlayerLevelExpBreakpointMultiplier(SELECTED_PLAYER_XP_SKILL, characterLevel, i)
        EndIf

        Int levelFlags = flags

        If i == 0
            levelFlags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_PLAYER_XP_BREAKPOINT_LEVELS[i] = AddSliderOption("Breakpoint " + (i + 1) + " Level", level, "{0}", levelFlags)

        OPTION_PLAYER_XP_BREAKPOINT_MULTIPLIERS[i] = AddSliderOption("Multiplier @ Level " + level, HundredthsToFloat(multiplier), "x{2}", flags)

        i += 1

    EndWhile

    If PLAYER_XP_ALL_SKILLS
        AddEmptyOption()
        OPTION_PLAYER_XP_APPLY_ALL_BREAKPOINTS = AddTextOption("Apply Table to All Skills", "Apply", flags)
    EndIf

EndFunction


; ============================================================================
; Draw Perks at Level Up page
; ============================================================================

Function DrawPerksAtLevelUpPage(Bool enabled)

    SetCursorFillMode(TOP_TO_BOTTOM)

    Int flags = OPTION_FLAG_NONE

    If !enabled
        flags = OPTION_FLAG_DISABLED
    EndIf

    AddHeaderOption("Perks at Level Up")

    AddTextOption("Reward Model", "Cumulative", flags)

    AddEmptyOption()

    AddHeaderOption("Add / Remove Breakpoint")

    OPTION_PERKS_NEW_BREAKPOINT_LEVEL = AddSliderOption("New Breakpoint Level", NEW_PERKS_BREAKPOINT_LEVEL, "{0}", flags)

    OPTION_PERKS_ADD_BREAKPOINT = AddTextOption("Add Breakpoint", "Add", flags)

    Int count = UncapperMCM.GetPerksAtLevelUpBreakpointCount()

    ACTIVE_PERKS_BREAKPOINT_COUNT = count

    Int removeFlags = flags

    If count <= 1
        removeFlags = OPTION_FLAG_DISABLED
    EndIf

    OPTION_PERKS_REMOVE_BREAKPOINT = AddMenuOption("Remove Breakpoint", "Choose...", removeFlags)

    AddEmptyOption()

    AddHeaderOption("Breakpoints")

    Int i = 0

    While i < count

        Int level = UncapperMCM.GetPerksAtLevelUpBreakpointLevel(i)
        Int perkValue = UncapperMCM.GetPerksAtLevelUpBreakpointValue(i)

        Int levelFlags = flags

        If i == 0
            levelFlags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_PERKS_BREAKPOINT_LEVELS[i] = AddSliderOption("Breakpoint " + (i + 1) + " Level", level, "{0}", levelFlags)

        OPTION_PERKS_BREAKPOINT_VALUES[i] = AddSliderOption("Perks @ Level " + level, HundredthsToFloat(perkValue), "{0}", flags)

        i += 1

    EndWhile

EndFunction


; ============================================================================
; Draw Attributes at Level Up page
; ============================================================================

Function DrawAttributesAtLevelUpPage(Bool enabled)

    SetCursorFillMode(TOP_TO_BOTTOM)

    Bool hookEnabled = UncapperMCM.GetIniUseAttributesAtLevelUp()

    Int flags = OPTION_FLAG_NONE

    If !enabled || !hookEnabled
        flags = OPTION_FLAG_DISABLED
    EndIf

    AddHeaderOption("Attributes at Level Up")

    If hookEnabled
        AddTextOption("Runtime Hook", "Enabled in INI")
    Else
        AddTextOption("Runtime Hook", "Disabled in INI", OPTION_FLAG_DISABLED)
    EndIf

    AddEmptyOption()

    AddHeaderOption("Attribute Gain")

    OPTION_ATTRIBUTE_CHOICE_SELECTOR = AddMenuOption("When Choosing", ATTRIBUTE_CHOICE_NAMES[SELECTED_ATTRIBUTE_CHOICE], flags)

    OPTION_ATTRIBUTE_TARGET_SELECTOR = AddMenuOption("Attribute Gained", ATTRIBUTE_TARGET_NAMES[SELECTED_ATTRIBUTE_TARGET], flags)

    AddEmptyOption()

    AddHeaderOption("Add / Remove Breakpoint")

    OPTION_ATTRIBUTE_NEW_BREAKPOINT_LEVEL = AddSliderOption("New Breakpoint Level", NEW_ATTRIBUTE_BREAKPOINT_LEVEL, "{0}", flags)

    OPTION_ATTRIBUTE_ADD_BREAKPOINT = AddTextOption("Add Breakpoint", "Add", flags)

    Int tableIndex = GetSelectedAttributeTableIndex()
    Int count = UncapperMCM.GetAttributeBreakpointCount(tableIndex)

    ACTIVE_ATTRIBUTE_BREAKPOINT_COUNT = count

    Int removeFlags = flags

    If count <= 1
        removeFlags = OPTION_FLAG_DISABLED
    EndIf

    OPTION_ATTRIBUTE_REMOVE_BREAKPOINT = AddMenuOption("Remove Breakpoint", "Choose...", removeFlags)

    AddEmptyOption()

    AddHeaderOption("Breakpoints")

    Int i = 0

    While i < count

        Int level = UncapperMCM.GetAttributeBreakpointLevel(tableIndex, i)
        Int attributeValue = UncapperMCM.GetAttributeBreakpointValue(tableIndex, i)

        Int levelFlags = flags

        If i == 0
            levelFlags = OPTION_FLAG_DISABLED
        EndIf

        OPTION_ATTRIBUTE_BREAKPOINT_LEVELS[i] = AddSliderOption("Breakpoint " + (i + 1) + " Level", level, "{0}", levelFlags)

        OPTION_ATTRIBUTE_BREAKPOINT_VALUES[i] = AddSliderOption("Gain @ Level " + level, attributeValue, "{0}", flags)

        i += 1

    EndWhile

EndFunction


; ============================================================================
; Select options
; ============================================================================

Event OnOptionSelect(Int option)

    If option == OPTION_ENABLED

        Bool newValue = !UncapperMCM.GetEnabled()

        If UncapperMCM.SetEnabled(newValue)
            SetToggleOptionValue(OPTION_ENABLED, newValue)
            ForcePageReset()
        Else
            ShowMessage("Unable to change the Uncapper MCM state. Check the SKSE log for details.", false)
            ForcePageReset()
        EndIf

        Return

    EndIf

    If option == OPTION_RESET_TO_INI

        Bool confirmed = ShowMessage("Reset all Uncapper MCM settings to the values currently loaded from SkyrimUncapper.ini?", true)

        If confirmed

            If UncapperMCM.ClearOverrides()
                XP_ALL_SKILLS = false
                PLAYER_XP_ALL_SKILLS = false
                ShowMessage("Settings restored from SkyrimUncapper.ini. Runtime overrides are now disabled.", false)
                ForcePageReset()
            Else
                ShowMessage("Unable to fully reset Uncapper MCM. Check the SKSE log for details.", false)
                ForcePageReset()
            EndIf

        EndIf

        Return

    EndIf

    If !UncapperMCM.GetEnabled()
        Return
    EndIf

    If option == OPTION_LEGENDARY_KEEP_LEVEL

        Bool currentKeep = UncapperMCM.GetLegendaryKeepSkillLevel()
        Bool newKeep = !currentKeep

        Bool currentHide = UncapperMCM.GetLegendaryHideButton()
        Int currentSkillLevel = UncapperMCM.GetLegendarySkillLevel()
        Int currentAfterLevel = UncapperMCM.GetLegendarySkillLevelAfter()

        If UncapperMCM.SetLegendarySettings(newKeep, currentHide, currentSkillLevel, currentAfterLevel)
            SetToggleOptionValue(OPTION_LEGENDARY_KEEP_LEVEL, newKeep)
        Else
            ShowMessage("Unable to change Keep Skill Level. When disabled, Skill Level After Legendary must be 0 or lower than the Legendary Skill Level.", false)
        EndIf

        Return

    EndIf


    If option == OPTION_LEGENDARY_HIDE_BUTTON

        Bool currentKeep = UncapperMCM.GetLegendaryKeepSkillLevel()
        Bool currentHide = UncapperMCM.GetLegendaryHideButton()
        Bool newHide = !currentHide

        Int currentSkillLevel = UncapperMCM.GetLegendarySkillLevel()
        Int currentAfterLevel = UncapperMCM.GetLegendarySkillLevelAfter()

        If UncapperMCM.SetLegendarySettings(currentKeep, newHide, currentSkillLevel, currentAfterLevel)
            SetToggleOptionValue(OPTION_LEGENDARY_HIDE_BUTTON, newHide)
        EndIf

        Return

    EndIf


    If option == OPTION_ENCHANT_LINEAR_CHARGE

        Bool newValue = !UncapperMCM.GetEnchantUseLinearCharge()
        UncapperMCM.SetEnchantUseLinearCharge(newValue)
        SetToggleOptionValue(OPTION_ENCHANT_LINEAR_CHARGE, newValue)
        Return

    EndIf

    If option == OPTION_XP_ADD_BREAKPOINT

        Bool added = false

        If XP_ALL_SKILLS
            added = AddBulkXpBreakpoint(CURRENT_XP_CHARACTER_LEVEL, NEW_XP_BREAKPOINT_LEVEL)
        Else
            added = UncapperMCM.AddSkillExpBreakpoint(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, NEW_XP_BREAKPOINT_LEVEL, 100, 100)
        EndIf

        If added
            ForcePageReset()
        Else
            ShowMessage("Unable to add this breakpoint. The level may already exist, be outside 1-500, or the table may already contain 32 breakpoints.", false)
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_ADD_BREAKPOINT

        Bool added = false

        If PLAYER_XP_ALL_SKILLS
            added = AddBulkPlayerXpBreakpoint(CURRENT_PLAYER_XP_CHARACTER_LEVEL, NEW_PLAYER_XP_BREAKPOINT_LEVEL)
        Else
            added = UncapperMCM.AddPlayerLevelExpBreakpoint(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, NEW_PLAYER_XP_BREAKPOINT_LEVEL, 100)
        EndIf

        If added
            ForcePageReset()
        Else
            ShowMessage("Unable to add this breakpoint. The level may already exist, be outside 1-500, or the table may already contain 32 breakpoints.", false)
        EndIf

        Return

    EndIf

    If option == OPTION_PERKS_ADD_BREAKPOINT

        Bool added = UncapperMCM.AddPerksAtLevelUpBreakpoint(NEW_PERKS_BREAKPOINT_LEVEL, 100)

        If added
            ForcePageReset()
        Else
            ShowMessage("Unable to add this breakpoint. The level may already exist, be outside 1-500, or the table may already contain 32 breakpoints.", false)
        EndIf

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_ADD_BREAKPOINT

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int defaultValue = GetAttributeValueForNewBreakpoint(tableIndex, NEW_ATTRIBUTE_BREAKPOINT_LEVEL)

        Bool added = UncapperMCM.AddAttributeBreakpoint(tableIndex, NEW_ATTRIBUTE_BREAKPOINT_LEVEL, defaultValue)

        If added
            ForcePageReset()
        Else
            ShowMessage("Unable to add this breakpoint. The level may already exist, be outside 1-500, or the table may already contain 32 breakpoints.", false)
        EndIf

        Return

    EndIf


    If option == OPTION_XP_APPLY_ALL_BASE

        Bool confirmed = ShowMessage("Apply these Skill XP base multipliers to all 18 skills? Existing individual base multipliers will be replaced.", true)

        If confirmed
            If ApplySkillXpBaseToAll()
                ShowMessage("Skill XP multipliers applied to all 18 skills.", false)
            Else
                ShowMessage("Unable to apply the Skill XP multipliers to all skills.", false)
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_XP_APPLY_ALL_BREAKPOINTS

        Bool confirmed = ShowMessage("Apply this breakpoint table to all 18 skills? Existing individual breakpoint tables on this page will be replaced.", true)

        If confirmed
            If ApplySkillXpBreakpointsToAll(CURRENT_XP_CHARACTER_LEVEL)
                ShowMessage("Breakpoint table applied to all 18 skills.", false)
            Else
                ShowMessage("Unable to apply the breakpoint table to all skills.", false)
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_APPLY_ALL_BASE

        Bool confirmed = ShowMessage("Apply this Player Level XP multiplier to all 18 skills? Existing individual multipliers will be replaced.", true)

        If confirmed
            If ApplyPlayerXpBaseToAll()
                ShowMessage("Player Level XP multiplier applied to all 18 skills.", false)
            Else
                ShowMessage("Unable to apply the Player Level XP multiplier to all skills.", false)
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_APPLY_ALL_BREAKPOINTS

        Bool confirmed = ShowMessage("Apply this Player Level XP breakpoint table to all 18 skills? Existing individual breakpoint tables on this page will be replaced.", true)

        If confirmed
            If ApplyPlayerXpBreakpointsToAll(CURRENT_PLAYER_XP_CHARACTER_LEVEL)
                ShowMessage("Player Level XP breakpoint table applied to all 18 skills.", false)
            Else
                ShowMessage("Unable to apply the Player Level XP breakpoint table to all skills.", false)
            EndIf
        EndIf

        Return

    EndIf

EndEvent


; ============================================================================
; Menu options
; ============================================================================

Event OnOptionMenuOpen(Int option)

    If !UncapperMCM.GetEnabled()
        Return
    EndIf

    If option == OPTION_XP_SKILL_SELECTOR

        SetMenuDialogOptions(SKILL_SELECTOR_NAMES)

        If XP_ALL_SKILLS
            SetMenuDialogStartIndex(0)
        Else
            SetMenuDialogStartIndex(SELECTED_XP_SKILL + 1)
        EndIf

        SetMenuDialogDefaultIndex(0)
        Return

    EndIf

    If option == OPTION_XP_REMOVE_BREAKPOINT

        Int count = 0

        If XP_ALL_SKILLS
            count = GetBulkXpCount(CURRENT_XP_CHARACTER_LEVEL)
        Else
            count = UncapperMCM.GetSkillExpBreakpointCount(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL)
        EndIf

        If count <= 1
            Return
        EndIf

        String[] options = Utility.CreateStringArray(count - 1)
        Int i = 1

        While i < count

            Int level = 0

            If XP_ALL_SKILLS
                level = GetBulkXpLevel(CURRENT_XP_CHARACTER_LEVEL, i)
            Else
                level = UncapperMCM.GetSkillExpBreakpointLevel(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, i)
            EndIf

            options[i - 1] = "Level " + level
            i += 1

        EndWhile

        SetMenuDialogOptions(options)
        SetMenuDialogStartIndex(0)
        SetMenuDialogDefaultIndex(0)
        Return

    EndIf

    If option == OPTION_PLAYER_XP_SKILL_SELECTOR

        SetMenuDialogOptions(SKILL_SELECTOR_NAMES)

        If PLAYER_XP_ALL_SKILLS
            SetMenuDialogStartIndex(0)
        Else
            SetMenuDialogStartIndex(SELECTED_PLAYER_XP_SKILL + 1)
        EndIf

        SetMenuDialogDefaultIndex(0)
        Return

    EndIf

    If option == OPTION_PLAYER_XP_REMOVE_BREAKPOINT

        Int count = 0

        If PLAYER_XP_ALL_SKILLS
            count = GetBulkPlayerXpCount(CURRENT_PLAYER_XP_CHARACTER_LEVEL)
        Else
            count = UncapperMCM.GetPlayerLevelExpBreakpointCount(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL)
        EndIf

        If count <= 1
            Return
        EndIf

        String[] options = Utility.CreateStringArray(count - 1)
        Int i = 1

        While i < count

            Int level = 0

            If PLAYER_XP_ALL_SKILLS
                level = GetBulkPlayerXpLevel(CURRENT_PLAYER_XP_CHARACTER_LEVEL, i)
            Else
                level = UncapperMCM.GetPlayerLevelExpBreakpointLevel(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, i)
            EndIf

            options[i - 1] = "Level " + level
            i += 1

        EndWhile

        SetMenuDialogOptions(options)
        SetMenuDialogStartIndex(0)
        SetMenuDialogDefaultIndex(0)

    EndIf

    If option == OPTION_PERKS_REMOVE_BREAKPOINT

        Int count = UncapperMCM.GetPerksAtLevelUpBreakpointCount()

        If count <= 1
            Return
        EndIf

        String[] options = Utility.CreateStringArray(count - 1)

        Int i = 1

        While i < count

            Int level = UncapperMCM.GetPerksAtLevelUpBreakpointLevel(i)

            options[i - 1] = "Level " + level

            i += 1

        EndWhile

        SetMenuDialogOptions(options)
        SetMenuDialogStartIndex(0)
        SetMenuDialogDefaultIndex(0)

        Return

    EndIf

    If option == OPTION_ATTRIBUTE_CHOICE_SELECTOR

        SetMenuDialogOptions(ATTRIBUTE_CHOICE_NAMES)
        SetMenuDialogStartIndex(SELECTED_ATTRIBUTE_CHOICE)
        SetMenuDialogDefaultIndex(0)

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_TARGET_SELECTOR

        SetMenuDialogOptions(ATTRIBUTE_TARGET_NAMES)
        SetMenuDialogStartIndex(SELECTED_ATTRIBUTE_TARGET)
        SetMenuDialogDefaultIndex(0)

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_REMOVE_BREAKPOINT

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int count = UncapperMCM.GetAttributeBreakpointCount(tableIndex)

        If count <= 1
            Return
        EndIf

        String[] options = Utility.CreateStringArray(count - 1)

        Int i = 1

        While i < count

            Int level = UncapperMCM.GetAttributeBreakpointLevel(tableIndex, i)

            options[i - 1] = "Level " + level

            i += 1

        EndWhile

        SetMenuDialogOptions(options)
        SetMenuDialogStartIndex(0)
        SetMenuDialogDefaultIndex(0)

        Return

    EndIf

EndEvent


Event OnOptionMenuAccept(Int option, Int index)

    If !UncapperMCM.GetEnabled()
        Return
    EndIf

    If option == OPTION_XP_SKILL_SELECTOR

        If index == 0

            If !XP_ALL_SKILLS
                CopySkillXpToBulk(SELECTED_XP_SKILL)
            EndIf

            XP_ALL_SKILLS = true
            SetMenuOptionValue(option, "All Skills")
            ForcePageReset()

        ElseIf index > 0 && index <= 18

            XP_ALL_SKILLS = false
            SELECTED_XP_SKILL = index - 1
            SetMenuOptionValue(option, SKILL_NAMES[SELECTED_XP_SKILL])
            ForcePageReset()

        EndIf

        Return

    EndIf

    If option == OPTION_XP_REMOVE_BREAKPOINT

        Bool removed = false

        If XP_ALL_SKILLS
            removed = RemoveBulkXpBreakpoint(CURRENT_XP_CHARACTER_LEVEL, index + 1)
        Else
            removed = UncapperMCM.RemoveSkillExpBreakpoint(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, index + 1)
        EndIf

        If removed
            ForcePageReset()
        Else
            ShowMessage("Unable to remove this breakpoint. The level 0 breakpoint is required.", false)
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_SKILL_SELECTOR

        If index == 0

            If !PLAYER_XP_ALL_SKILLS
                CopyPlayerXpToBulk(SELECTED_PLAYER_XP_SKILL)
            EndIf

            PLAYER_XP_ALL_SKILLS = true
            SetMenuOptionValue(option, "All Skills")
            ForcePageReset()

        ElseIf index > 0 && index <= 18

            PLAYER_XP_ALL_SKILLS = false
            SELECTED_PLAYER_XP_SKILL = index - 1
            SetMenuOptionValue(option, SKILL_NAMES[SELECTED_PLAYER_XP_SKILL])
            ForcePageReset()

        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_REMOVE_BREAKPOINT

        Bool removed = false

        If PLAYER_XP_ALL_SKILLS
            removed = RemoveBulkPlayerXpBreakpoint(CURRENT_PLAYER_XP_CHARACTER_LEVEL, index + 1)
        Else
            removed = UncapperMCM.RemovePlayerLevelExpBreakpoint(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, index + 1)
        EndIf

        If removed
            ForcePageReset()
        Else
            ShowMessage("Unable to remove this breakpoint. The level 0 breakpoint is required.", false)
        EndIf

    EndIf

    If option == OPTION_PERKS_REMOVE_BREAKPOINT

        Bool removed = UncapperMCM.RemovePerksAtLevelUpBreakpoint(index + 1)

        If removed
            ForcePageReset()
        Else
            ShowMessage("Unable to remove this breakpoint. The level 0 breakpoint is required.", false)
        EndIf

        Return

    EndIf

    If option == OPTION_ATTRIBUTE_CHOICE_SELECTOR

        If index >= 0 && index < 3
            SELECTED_ATTRIBUTE_CHOICE = index
            SetMenuOptionValue(option, ATTRIBUTE_CHOICE_NAMES[index])
            ForcePageReset()
        EndIf

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_TARGET_SELECTOR

        If index >= 0 && index < 4
            SELECTED_ATTRIBUTE_TARGET = index
            SetMenuOptionValue(option, ATTRIBUTE_TARGET_NAMES[index])
            ForcePageReset()
        EndIf

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_REMOVE_BREAKPOINT

        Int tableIndex = GetSelectedAttributeTableIndex()

        Bool removed = UncapperMCM.RemoveAttributeBreakpoint(tableIndex, index + 1)

        If removed
            ForcePageReset()
        Else
            ShowMessage("Unable to remove this breakpoint. The level 0 breakpoint is required.", false)
        EndIf

        Return

    EndIf

EndEvent


; ============================================================================
; Slider open
; ============================================================================

Event OnOptionSliderOpen(Int option)

    If !UncapperMCM.GetEnabled()
        Return
    EndIf


    If option == OPTION_LEGENDARY_SKILL_LEVEL

        Int legendarySkillLevel = UncapperMCM.GetLegendarySkillLevel()

        SetSliderDialogStartValue(legendarySkillLevel)
        SetSliderDialogDefaultValue(100.0)
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    If option == OPTION_LEGENDARY_AFTER_LEVEL

        Int legendaryAfterLevel = UncapperMCM.GetLegendarySkillLevelAfter()

        SetSliderDialogStartValue(legendaryAfterLevel)
        SetSliderDialogDefaultValue(0.0)
        SetSliderDialogRange(0.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    If option == OPTION_ENCHANT_MAGNITUDE_CAP

        SetSliderDialogStartValue(UncapperMCM.GetEnchantMagnitudeCap())
        SetSliderDialogDefaultValue(UncapperMCM.GetEnchantMagnitudeCap())
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return


    ElseIf option == OPTION_ENCHANT_CHARGE_CAP

        SetSliderDialogStartValue(UncapperMCM.GetEnchantChargeCap())
        SetSliderDialogDefaultValue(UncapperMCM.GetEnchantChargeCap())
        SetSliderDialogRange(1.0, 199.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    If option == OPTION_XP_BASE_MULTIPLIER

        If XP_ALL_SKILLS
            SetSliderDialogStartValue(HundredthsToFloat(BULK_XP_BASE_MULTIPLIER))
        Else
            SetSliderDialogStartValue(HundredthsToFloat(UncapperMCM.GetSkillExpBaseMultiplier(SELECTED_XP_SKILL)))
        EndIf
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    If option == OPTION_XP_OFFSET_MULTIPLIER

        If XP_ALL_SKILLS
            SetSliderDialogStartValue(HundredthsToFloat(BULK_XP_OFFSET_MULTIPLIER))
        Else
            SetSliderDialogStartValue(HundredthsToFloat(UncapperMCM.GetSkillExpOffsetMultiplier(SELECTED_XP_SKILL)))
        EndIf
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    If option == OPTION_XP_NEW_BREAKPOINT_LEVEL

        SetSliderDialogStartValue(NEW_XP_BREAKPOINT_LEVEL)
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    Int breakpointIndex = FindXpBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int level = 0

        If XP_ALL_SKILLS
            level = GetBulkXpLevel(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        Else
            level = UncapperMCM.GetSkillExpBreakpointLevel(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        EndIf

        SetSliderDialogStartValue(level)
        SetSliderDialogDefaultValue(level)
        SetSliderDialogRange(0.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindXpBreakpointBaseOption(option)

    If breakpointIndex >= 0

        Int baseMult = 100

        If XP_ALL_SKILLS
            baseMult = GetBulkXpBase(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        Else
            baseMult = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        EndIf

        SetSliderDialogStartValue(HundredthsToFloat(baseMult))
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    breakpointIndex = FindXpBreakpointOffsetOption(option)

    If breakpointIndex >= 0

        Int offsetMult = 100

        If XP_ALL_SKILLS
            offsetMult = GetBulkXpOffset(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        Else
            offsetMult = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
        EndIf

        SetSliderDialogStartValue(HundredthsToFloat(offsetMult))
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    If option == OPTION_PLAYER_XP_MULTIPLIER

        If PLAYER_XP_ALL_SKILLS
            SetSliderDialogStartValue(HundredthsToFloat(BULK_PLAYER_XP_MULTIPLIER))
        Else
            SetSliderDialogStartValue(HundredthsToFloat(UncapperMCM.GetPlayerLevelExpMultiplier(SELECTED_PLAYER_XP_SKILL)))
        EndIf
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    If option == OPTION_PLAYER_XP_NEW_BREAKPOINT_LEVEL

        SetSliderDialogStartValue(NEW_PLAYER_XP_BREAKPOINT_LEVEL)
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindPlayerXpBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int level = 0

        If PLAYER_XP_ALL_SKILLS
            level = GetBulkPlayerXpLevel(CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)
        Else
            level = UncapperMCM.GetPlayerLevelExpBreakpointLevel(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)
        EndIf

        SetSliderDialogStartValue(level)
        SetSliderDialogDefaultValue(level)
        SetSliderDialogRange(0.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindPlayerXpBreakpointMultiplierOption(option)

    If breakpointIndex >= 0

        Int multiplier = 100

        If PLAYER_XP_ALL_SKILLS
            multiplier = GetBulkPlayerXpMultiplier(CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)
        Else
            multiplier = UncapperMCM.GetPlayerLevelExpBreakpointMultiplier(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)
        EndIf

        SetSliderDialogStartValue(HundredthsToFloat(multiplier))
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(0.01)

        Return

    EndIf


    If option == OPTION_PERKS_NEW_BREAKPOINT_LEVEL

        SetSliderDialogStartValue(NEW_PERKS_BREAKPOINT_LEVEL)
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindPerksBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int perkLevel = UncapperMCM.GetPerksAtLevelUpBreakpointLevel(breakpointIndex)

        SetSliderDialogStartValue(perkLevel)
        SetSliderDialogDefaultValue(perkLevel)
        SetSliderDialogRange(0.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindPerksBreakpointValueOption(option)

    If breakpointIndex >= 0

        Int perkValue = UncapperMCM.GetPerksAtLevelUpBreakpointValue(breakpointIndex)

        SetSliderDialogStartValue(HundredthsToFloat(perkValue))
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_NEW_BREAKPOINT_LEVEL

        SetSliderDialogStartValue(NEW_ATTRIBUTE_BREAKPOINT_LEVEL)
        SetSliderDialogDefaultValue(1.0)
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindAttributeBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int attributeLevel = UncapperMCM.GetAttributeBreakpointLevel(tableIndex, breakpointIndex)

        SetSliderDialogStartValue(attributeLevel)
        SetSliderDialogDefaultValue(attributeLevel)
        SetSliderDialogRange(0.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    breakpointIndex = FindAttributeBreakpointValueOption(option)

    If breakpointIndex >= 0

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int attributeValue = UncapperMCM.GetAttributeBreakpointValue(tableIndex, breakpointIndex)

        SetSliderDialogStartValue(attributeValue)
        SetSliderDialogDefaultValue(attributeValue)
        SetSliderDialogRange(0.0, 100.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    Int skillIndex = FindSkillCapOption(option)

    If skillIndex >= 0

        SetSliderDialogStartValue(UncapperMCM.GetSkillCap(skillIndex))
        SetSliderDialogDefaultValue(UncapperMCM.GetSkillCap(skillIndex))
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

        Return

    EndIf


    skillIndex = FindFormulaCapOption(option)

    If skillIndex >= 0

        SetSliderDialogStartValue(UncapperMCM.GetFormulaCap(skillIndex))
        SetSliderDialogDefaultValue(UncapperMCM.GetFormulaCap(skillIndex))
        SetSliderDialogRange(1.0, 500.0)
        SetSliderDialogInterval(1.0)

    EndIf

EndEvent


; ============================================================================
; Slider accept
; ============================================================================

Event OnOptionSliderAccept(Int option, Float value)

    If !UncapperMCM.GetEnabled()
        Return
    EndIf

    Int newValue = value as Int

    If option == OPTION_LEGENDARY_SKILL_LEVEL

        Bool currentKeep = UncapperMCM.GetLegendaryKeepSkillLevel()
        Bool currentHide = UncapperMCM.GetLegendaryHideButton()
        Int currentAfterLevel = UncapperMCM.GetLegendarySkillLevelAfter()

        If UncapperMCM.SetLegendarySettings(currentKeep, currentHide, newValue, currentAfterLevel)
            SetSliderOptionValue(OPTION_LEGENDARY_SKILL_LEVEL, newValue, "{0}")

            Int highestCap = GetHighestSkillCap()

            If newValue > highestCap
                ShowMessage("The Legendary Skill Level is above your current highest Skill Cap. Legendary may be unreachable through normal skill progression.", false)
            EndIf
        Else
            ShowMessage("Unable to change the Legendary Skill Level. When Keep Skill Level is disabled, Skill Level After Legendary must be 0 or lower than the Legendary Skill Level.", false)
        EndIf

        Return

    EndIf


    If option == OPTION_LEGENDARY_AFTER_LEVEL

        Bool currentKeep = UncapperMCM.GetLegendaryKeepSkillLevel()
        Bool currentHide = UncapperMCM.GetLegendaryHideButton()
        Int currentSkillLevel = UncapperMCM.GetLegendarySkillLevel()

        If UncapperMCM.SetLegendarySettings(currentKeep, currentHide, currentSkillLevel, newValue)
            SetSliderOptionValue(OPTION_LEGENDARY_AFTER_LEVEL, newValue, "{0}")
        Else
            ShowMessage("Unable to change Skill Level After Legendary. When Keep Skill Level is disabled, this value must be 0 or lower than the Legendary Skill Level.", false)
        EndIf

        Return

    EndIf


    If option == OPTION_ENCHANT_MAGNITUDE_CAP

        If UncapperMCM.SetEnchantMagnitudeCap(newValue)
            SetSliderOptionValue(OPTION_ENCHANT_MAGNITUDE_CAP, newValue, "{0}")

            If newValue > 400
                ShowMessage("Values above 400 are experimental. The hard limit is 500.", false)
            EndIf
        EndIf

        Return

    ElseIf option == OPTION_ENCHANT_CHARGE_CAP

        If UncapperMCM.SetEnchantChargeCap(newValue)
            SetSliderOptionValue(OPTION_ENCHANT_CHARGE_CAP, newValue, "{0}")
        EndIf

        Return

    EndIf

    If option == OPTION_XP_BASE_MULTIPLIER

        Int hundredths = FloatToHundredths(value)

        If XP_ALL_SKILLS
            BULK_XP_BASE_MULTIPLIER = hundredths
            SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
        Else
            Int offsetHundredths = UncapperMCM.GetSkillExpOffsetMultiplier(SELECTED_XP_SKILL)

            If UncapperMCM.SetSkillExpBaseMultipliers(SELECTED_XP_SKILL, hundredths, offsetHundredths)
                SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_XP_OFFSET_MULTIPLIER

        Int hundredths = FloatToHundredths(value)

        If XP_ALL_SKILLS
            BULK_XP_OFFSET_MULTIPLIER = hundredths
            SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
        Else
            Int baseHundredths = UncapperMCM.GetSkillExpBaseMultiplier(SELECTED_XP_SKILL)

            If UncapperMCM.SetSkillExpBaseMultipliers(SELECTED_XP_SKILL, baseHundredths, hundredths)
                SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_XP_NEW_BREAKPOINT_LEVEL

        NEW_XP_BREAKPOINT_LEVEL = newValue
        SetSliderOptionValue(option, newValue, "{0}")
        Return

    EndIf

    Int breakpointIndex = FindXpBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Bool changed = false

        If XP_ALL_SKILLS
            changed = SetBulkXpBreakpointLevel(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, newValue)
        Else
            Int currentBase = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
            Int currentOffset = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
            changed = UncapperMCM.SetSkillExpBreakpoint(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, newValue, currentBase, currentOffset)
        EndIf

        If changed
            ForcePageReset()
        Else
            ShowMessage("Unable to change this breakpoint level. Level 0 is required and another breakpoint may already use that level.", false)
        EndIf

        Return

    EndIf

    breakpointIndex = FindXpBreakpointBaseOption(option)

    If breakpointIndex >= 0

        Int baseMult = FloatToHundredths(value)

        If XP_ALL_SKILLS
            SetBulkXpBase(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, baseMult)
            SetSliderOptionValue(option, HundredthsToFloat(baseMult), "x{2}")
        Else
            Int level = UncapperMCM.GetSkillExpBreakpointLevel(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
            Int offsetMult = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)

            If UncapperMCM.SetSkillExpBreakpoint(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, level, baseMult, offsetMult)
                SetSliderOptionValue(option, HundredthsToFloat(baseMult), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    breakpointIndex = FindXpBreakpointOffsetOption(option)

    If breakpointIndex >= 0

        Int offsetMult = FloatToHundredths(value)

        If XP_ALL_SKILLS
            SetBulkXpOffset(CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, offsetMult)
            SetSliderOptionValue(option, HundredthsToFloat(offsetMult), "x{2}")
        Else
            Int level = UncapperMCM.GetSkillExpBreakpointLevel(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)
            Int baseMult = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex)

            If UncapperMCM.SetSkillExpBreakpoint(SELECTED_XP_SKILL, CURRENT_XP_CHARACTER_LEVEL, breakpointIndex, level, baseMult, offsetMult)
                SetSliderOptionValue(option, HundredthsToFloat(offsetMult), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_MULTIPLIER

        Int hundredths = FloatToHundredths(value)

        If PLAYER_XP_ALL_SKILLS
            BULK_PLAYER_XP_MULTIPLIER = hundredths
            SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
        Else
            If UncapperMCM.SetPlayerLevelExpMultiplier(SELECTED_PLAYER_XP_SKILL, hundredths)
                SetSliderOptionValue(option, HundredthsToFloat(hundredths), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_PLAYER_XP_NEW_BREAKPOINT_LEVEL

        NEW_PLAYER_XP_BREAKPOINT_LEVEL = newValue
        SetSliderOptionValue(option, newValue, "{0}")
        Return

    EndIf

    breakpointIndex = FindPlayerXpBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Bool changed = false

        If PLAYER_XP_ALL_SKILLS
            changed = SetBulkPlayerXpBreakpointLevel(CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex, newValue)
        Else
            Int currentMultiplier = UncapperMCM.GetPlayerLevelExpBreakpointMultiplier(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)
            changed = UncapperMCM.SetPlayerLevelExpBreakpoint(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex, newValue, currentMultiplier)
        EndIf

        If changed
            ForcePageReset()
        Else
            ShowMessage("Unable to change this breakpoint level. Level 0 is required and another breakpoint may already use that level.", false)
        EndIf

        Return

    EndIf

    breakpointIndex = FindPlayerXpBreakpointMultiplierOption(option)

    If breakpointIndex >= 0

        Int multiplier = FloatToHundredths(value)

        If PLAYER_XP_ALL_SKILLS
            SetBulkPlayerXpMultiplier(CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex, multiplier)
            SetSliderOptionValue(option, HundredthsToFloat(multiplier), "x{2}")
        Else
            Int level = UncapperMCM.GetPlayerLevelExpBreakpointLevel(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex)

            If UncapperMCM.SetPlayerLevelExpBreakpoint(SELECTED_PLAYER_XP_SKILL, CURRENT_PLAYER_XP_CHARACTER_LEVEL, breakpointIndex, level, multiplier)
                SetSliderOptionValue(option, HundredthsToFloat(multiplier), "x{2}")
            EndIf
        EndIf

        Return

    EndIf

    If option == OPTION_PERKS_NEW_BREAKPOINT_LEVEL

        NEW_PERKS_BREAKPOINT_LEVEL = newValue
        SetSliderOptionValue(option, newValue, "{0}")

        Return

    EndIf


    breakpointIndex = FindPerksBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int currentPerkValue = UncapperMCM.GetPerksAtLevelUpBreakpointValue(breakpointIndex)

        If UncapperMCM.SetPerksAtLevelUpBreakpoint(breakpointIndex, newValue, currentPerkValue)
            ForcePageReset()
        Else
            ShowMessage("Unable to change this breakpoint level. Level 0 is required and another breakpoint may already use that level.", false)
        EndIf

        Return

    EndIf


    breakpointIndex = FindPerksBreakpointValueOption(option)

    If breakpointIndex >= 0

        Int perkHundredths = FloatToHundredths(value)
        Int perkLevel = UncapperMCM.GetPerksAtLevelUpBreakpointLevel(breakpointIndex)

        If UncapperMCM.SetPerksAtLevelUpBreakpoint(breakpointIndex, perkLevel, perkHundredths)
            SetSliderOptionValue(option, HundredthsToFloat(perkHundredths), "{0}")
        EndIf

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_NEW_BREAKPOINT_LEVEL

        NEW_ATTRIBUTE_BREAKPOINT_LEVEL = newValue
        SetSliderOptionValue(option, newValue, "{0}")

        Return

    EndIf


    breakpointIndex = FindAttributeBreakpointLevelOption(option)

    If breakpointIndex >= 0

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int currentAttributeValue = UncapperMCM.GetAttributeBreakpointValue(tableIndex, breakpointIndex)

        If UncapperMCM.SetAttributeBreakpoint(tableIndex, breakpointIndex, newValue, currentAttributeValue)
            ForcePageReset()
        Else
            ShowMessage("Unable to change this breakpoint level. Level 0 is required and another breakpoint may already use that level.", false)
        EndIf

        Return

    EndIf


    breakpointIndex = FindAttributeBreakpointValueOption(option)

    If breakpointIndex >= 0

        Int tableIndex = GetSelectedAttributeTableIndex()
        Int attributeLevel = UncapperMCM.GetAttributeBreakpointLevel(tableIndex, breakpointIndex)

        If UncapperMCM.SetAttributeBreakpoint(tableIndex, breakpointIndex, attributeLevel, newValue)
            SetSliderOptionValue(option, newValue, "{0}")
        EndIf

        Return

    EndIf


    Int skillIndex = FindSkillCapOption(option)

    If skillIndex >= 0

        If UncapperMCM.SetSkillCap(skillIndex, newValue)
            SetSliderOptionValue(option, newValue, "{0}")

            If newValue > 400
                ShowMessage("Values above 400 are experimental. The hard limit is 500.", false)
            EndIf
        EndIf

        Return

    EndIf

    skillIndex = FindFormulaCapOption(option)

    If skillIndex >= 0

        If UncapperMCM.SetFormulaCap(skillIndex, newValue)
            SetSliderOptionValue(option, newValue, "{0}")

            If newValue > 400
                ShowMessage("Values above 400 are experimental. The hard limit is 500.", false)
            EndIf
        EndIf

    EndIf

EndEvent


; ============================================================================
; Help text
; ============================================================================

Event OnOptionHighlight(Int option)

    If option == OPTION_ENABLED

        SetInfoText("Enable per-save runtime overrides. When disabled, SkyrimUncapper.ini remains in control.")

        Return

    EndIf


    If option == OPTION_RESET_TO_INI

        SetInfoText("Restore all MCM values from SkyrimUncapper.ini and disable runtime overrides.")

        Return

    EndIf


    If option == OPTION_LEGENDARY_HOOK_STATUS

        If UncapperMCM.GetIniUseLegendarySettings()
            SetInfoText("Legendary hooks were enabled by bUseLegendarySettings in SkyrimUncapper.ini when Skyrim started. This status cannot be changed at runtime.")
        Else
            SetInfoText("Legendary hooks are disabled. Set bUseLegendarySettings=true in SkyrimUncapper.ini and restart Skyrim to use Legendary runtime settings.")
        EndIf

        Return

    EndIf


    If option == OPTION_LEGENDARY_SKILL_LEVEL

        SetInfoText("Base skill level required before a vanilla skill can become Legendary. The threshold applies globally to all 18 vanilla skills. It is independent from Skill Caps and can be set from 1 to 500.")

        Return

    EndIf


    If option == OPTION_LEGENDARY_AFTER_LEVEL

        SetInfoText("Skill level applied after making a skill Legendary. 0 uses Skyrim's native Legendary reset value. When Keep Skill Level is disabled, a non-zero value must be lower than the Legendary Skill Level.")

        Return

    EndIf


    If option == OPTION_LEGENDARY_KEEP_LEVEL

        SetInfoText("Keep the skill at its current level after making it Legendary. Skill Level After Legendary is then ignored, and the skill may remain immediately eligible to become Legendary again.")

        Return

    EndIf


    If option == OPTION_LEGENDARY_HIDE_BUTTON

        SetInfoText("Hide the Legendary button or hint in the vanilla Skills menu. This only affects visibility: the Legendary action may still be available through its keyboard input when the skill meets the threshold.")

        Return

    EndIf


    If option == OPTION_LEGENDARY_HIGHEST_SKILL_CAP

        SetInfoText("Highest Skill Cap currently configured across the 18 vanilla skills. This is informational only and does not automatically change the Legendary Skill Level.")

        Return

    EndIf


    If option == OPTION_ENCHANT_MAGNITUDE_CAP

        SetInfoText("Maximum Enchanting level used to calculate enchantment strength. For example, if set to 100, increasing Enchanting above 100 will not make enchantments stronger through this calculation.")

        Return

    EndIf


    If option == OPTION_ENCHANT_CHARGE_CAP

        SetInfoText("Maximum Enchanting level used to calculate weapon charge efficiency. Higher values can reduce charge cost per use. This value cannot exceed 199 because the original formula becomes unstable above that level.")

        Return

    EndIf


    If option == OPTION_ENCHANT_LINEAR_CHARGE

        SetInfoText("Uses Skyrim Uncapper's smoothed weapon charge formula. Despite the historical name, the formula is quadratic rather than truly linear. It reduces extreme charge scaling at high Enchanting levels, but can make weapon charges stronger at lower levels.")

        Return

    EndIf


    If option == OPTION_XP_SKILL_SELECTOR

        SetInfoText("Select one skill, or All Skills for bulk editing. All Skills creates a temporary template; changes are copied to all 18 skills only when you choose Apply.")

        Return

    EndIf


    If option == OPTION_XP_BASE_MULTIPLIER

        SetInfoText("Multiplies the main XP component gained by this skill. x1.00 keeps the original Skyrim Uncapper value unchanged.")

        Return

    EndIf


    If option == OPTION_XP_OFFSET_MULTIPLIER

        SetInfoText("Multiplies the additional offset component in Skyrim's skill XP formula. Most configurations can leave this at x1.00.")

        Return

    EndIf


    If option == OPTION_XP_NEW_BREAKPOINT_LEVEL

        SetInfoText("Choose the level where a new XP multiplier breakpoint should begin. Any integer level from 1 to 500 can be used.")

        Return

    EndIf


    If option == OPTION_XP_ADD_BREAKPOINT

        SetInfoText("Add a new breakpoint at the selected level. New breakpoints start at x1.00 base and x1.00 offset and can be edited below.")

        Return

    EndIf


    If option == OPTION_XP_REMOVE_BREAKPOINT

        SetInfoText("Remove one breakpoint from this table. The level 0 breakpoint is required and cannot be removed.")

        Return

    EndIf


    If FindXpBreakpointLevelOption(option) >= 0

        SetInfoText("Level where this breakpoint begins. Its multipliers remain active until the next higher breakpoint is reached. The level 0 breakpoint is required and cannot be moved.")

        Return

    EndIf


    If FindXpBreakpointBaseOption(option) >= 0

        SetInfoText("Base XP multiplier applied from this breakpoint onward, until another higher breakpoint replaces it.")

        Return

    EndIf


    If FindXpBreakpointOffsetOption(option) >= 0

        SetInfoText("Offset XP multiplier applied from this breakpoint onward. Most configurations can leave this at x1.00.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_SKILL_SELECTOR

        SetInfoText("Select one skill, or All Skills for bulk editing. All Skills creates a temporary template; changes are copied to all 18 skills only when you choose Apply.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_MULTIPLIER

        SetInfoText("Controls how much this skill contributes to player character leveling when the skill gains progress. x1.00 keeps the Skyrim Uncapper value unchanged.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_NEW_BREAKPOINT_LEVEL

        SetInfoText("Choose the level where a new player-level XP multiplier breakpoint should begin. Any integer level from 1 to 500 can be used.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_ADD_BREAKPOINT

        SetInfoText("Add a new player-level XP breakpoint at the selected level. New breakpoints start at x1.00 and can be edited below.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_REMOVE_BREAKPOINT

        SetInfoText("Remove one player-level XP breakpoint. The level 0 breakpoint is required and cannot be removed.")

        Return

    EndIf


    If FindPlayerXpBreakpointLevelOption(option) >= 0

        SetInfoText("Level where this player-level XP breakpoint begins. The level 0 breakpoint is required and cannot be moved.")

        Return

    EndIf


    If FindPlayerXpBreakpointMultiplierOption(option) >= 0

        SetInfoText("Multiplier applied to this skill's contribution toward player character level from this breakpoint onward, until the next higher breakpoint is reached.")

        Return

    EndIf


    If option == OPTION_XP_APPLY_ALL_BASE

        SetInfoText("Copy the currently displayed Skill XP base and offset multipliers to all 18 skills. Existing individual values will be replaced after confirmation.")

        Return

    EndIf


    If option == OPTION_XP_APPLY_ALL_BREAKPOINTS

        SetInfoText("Copy the current breakpoint template on this page to all 18 skills. Existing individual breakpoint tables for this page will be replaced after confirmation.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_APPLY_ALL_BASE

        SetInfoText("Copy the currently displayed Player Level XP multiplier to all 18 skills. Existing individual values will be replaced after confirmation.")

        Return

    EndIf


    If option == OPTION_PLAYER_XP_APPLY_ALL_BREAKPOINTS

        SetInfoText("Copy the current Player Level XP breakpoint template on this page to all 18 skills. Existing individual breakpoint tables for this page will be replaced after confirmation.")

        Return

    EndIf


    If option == OPTION_PERKS_NEW_BREAKPOINT_LEVEL

        SetInfoText("Choose the player level where a new perk-award breakpoint should begin. Any integer level from 1 to 500 can be used.")

        Return

    EndIf


    If option == OPTION_PERKS_ADD_BREAKPOINT

        SetInfoText("Add a new Perks at Level Up breakpoint. New breakpoints start at 1.00 perk per level and can be edited below.")

        Return

    EndIf


    If option == OPTION_PERKS_REMOVE_BREAKPOINT

        SetInfoText("Remove one Perks at Level Up breakpoint. The level 0 breakpoint is required and cannot be removed.")

        Return

    EndIf


    If FindPerksBreakpointLevelOption(option) >= 0

        SetInfoText("Player level where this perk-award breakpoint begins. The level 0 breakpoint is required and cannot be moved.")

        Return

    EndIf


    If FindPerksBreakpointValueOption(option) >= 0

        SetInfoText("Average perks awarded per player level from this breakpoint onward. Fractional values accumulate across levels; for example 1.50 produces 1, then 2, then 1, then 2 perks.")

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_CHOICE_SELECTOR

        SetInfoText("Select which attribute the player chooses on the level-up screen: Health, Magicka or Stamina.")

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_TARGET_SELECTOR

        SetInfoText("Select which actor value receives the configured gain when the chosen level-up attribute is selected.")

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_NEW_BREAKPOINT_LEVEL

        SetInfoText("Choose the player level where a new attribute-gain breakpoint begins. New breakpoints can be added from level 1 to 500.")

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_ADD_BREAKPOINT

        SetInfoText("Add a new breakpoint for the selected choice and gained attribute. The new breakpoint inherits the value active immediately before it.")

        Return

    EndIf


    If option == OPTION_ATTRIBUTE_REMOVE_BREAKPOINT

        SetInfoText("Remove one breakpoint from the selected attribute table. The level 0 breakpoint is required and cannot be removed.")

        Return

    EndIf


    If FindAttributeBreakpointLevelOption(option) >= 0

        SetInfoText("Player level where this attribute-gain breakpoint begins. The level 0 breakpoint is required and cannot be moved.")

        Return

    EndIf


    If FindAttributeBreakpointValueOption(option) >= 0

        SetInfoText("Amount added to the selected actor value at level-up. Values are whole points from 0 to 100 and remain active until the next breakpoint.")

        Return

    EndIf


    If FindSkillCapOption(option) >= 0

        SetInfoText("Maximum level this skill can actually reach. For example, a Skill Cap of 150 allows the skill to increase up to 150, but not beyond it. Values above 400 are experimental; the hard limit is 500.")

        Return

    EndIf


    If FindFormulaCapOption(option) >= 0

        SetInfoText("Maximum skill level used by game formulas. For example, if this is set to 100, a skill at 150 can still exist, but calculations that use this cap will treat it as 100. Values above 400 are experimental; the hard limit is 500.")

    EndIf

EndEvent


; ============================================================================
; Helpers
; ============================================================================

Int Function GetHighestSkillCap()

    Int highest = 0
    Int i = 0

    While i < 18

        Int currentCap = UncapperMCM.GetSkillCap(i)

        If currentCap > highest
            highest = currentCap
        EndIf

        i += 1

    EndWhile

    Return highest

EndFunction




Float Function HundredthsToFloat(Int value)

    Return (value as Float) / 100.0

EndFunction


Int Function FloatToHundredths(Float value)

    Return ((value * 100.0) + 0.5) as Int

EndFunction


Int Function FindSkillCapOption(Int option)

    If OPTION_SKILL_CAPS == None
        Return -1
    EndIf

    Int i = 0

    While i < 18

        If OPTION_SKILL_CAPS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindFormulaCapOption(Int option)

    If OPTION_FORMULA_CAPS == None
        Return -1
    EndIf

    Int i = 0

    While i < 18

        If OPTION_FORMULA_CAPS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindXpBreakpointLevelOption(Int option)

    If OPTION_XP_BREAKPOINT_LEVELS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_XP_BREAKPOINT_COUNT

        If OPTION_XP_BREAKPOINT_LEVELS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindXpBreakpointBaseOption(Int option)

    If OPTION_XP_BREAKPOINT_BASES == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_XP_BREAKPOINT_COUNT

        If OPTION_XP_BREAKPOINT_BASES[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindXpBreakpointOffsetOption(Int option)

    If OPTION_XP_BREAKPOINT_OFFSETS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_XP_BREAKPOINT_COUNT

        If OPTION_XP_BREAKPOINT_OFFSETS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindPlayerXpBreakpointLevelOption(Int option)

    If OPTION_PLAYER_XP_BREAKPOINT_LEVELS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_PLAYER_XP_BREAKPOINT_COUNT

        If OPTION_PLAYER_XP_BREAKPOINT_LEVELS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindPlayerXpBreakpointMultiplierOption(Int option)

    If OPTION_PLAYER_XP_BREAKPOINT_MULTIPLIERS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_PLAYER_XP_BREAKPOINT_COUNT

        If OPTION_PLAYER_XP_BREAKPOINT_MULTIPLIERS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction
; ============================================================================
; Attributes at Level Up helpers
; ============================================================================

Int Function GetSelectedAttributeTableIndex()

    Return (SELECTED_ATTRIBUTE_TARGET * 3) + SELECTED_ATTRIBUTE_CHOICE

EndFunction


Int Function GetAttributeValueForNewBreakpoint(Int tableIndex, Int newLevel)

    Int count = UncapperMCM.GetAttributeBreakpointCount(tableIndex)

    If count <= 0
        Return 0
    EndIf

    Int selectedValue = UncapperMCM.GetAttributeBreakpointValue(tableIndex, 0)

    Int i = 1

    While i < count

        Int breakpointLevel = UncapperMCM.GetAttributeBreakpointLevel(tableIndex, i)

        If breakpointLevel > newLevel
            Return selectedValue
        EndIf

        selectedValue = UncapperMCM.GetAttributeBreakpointValue(tableIndex, i)

        i += 1

    EndWhile

    Return selectedValue

EndFunction


Int Function FindAttributeBreakpointLevelOption(Int option)

    If OPTION_ATTRIBUTE_BREAKPOINT_LEVELS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_ATTRIBUTE_BREAKPOINT_COUNT

        If OPTION_ATTRIBUTE_BREAKPOINT_LEVELS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindAttributeBreakpointValueOption(Int option)

    If OPTION_ATTRIBUTE_BREAKPOINT_VALUES == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_ATTRIBUTE_BREAKPOINT_COUNT

        If OPTION_ATTRIBUTE_BREAKPOINT_VALUES[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


; ============================================================================
; Perks at Level Up helpers
; ============================================================================

Int Function FindPerksBreakpointLevelOption(Int option)

    If OPTION_PERKS_BREAKPOINT_LEVELS == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_PERKS_BREAKPOINT_COUNT

        If OPTION_PERKS_BREAKPOINT_LEVELS[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


Int Function FindPerksBreakpointValueOption(Int option)

    If OPTION_PERKS_BREAKPOINT_VALUES == None
        Return -1
    EndIf

    Int i = 0

    While i < ACTIVE_PERKS_BREAKPOINT_COUNT

        If OPTION_PERKS_BREAKPOINT_VALUES[i] == option
            Return i
        EndIf

        i += 1

    EndWhile

    Return -1

EndFunction


; ============================================================================
; Bulk selection helpers
; ============================================================================

String Function GetXpSelectionLabel()

    If XP_ALL_SKILLS
        Return "All Skills"
    EndIf

    Return SKILL_NAMES[SELECTED_XP_SKILL]

EndFunction


String Function GetPlayerXpSelectionLabel()

    If PLAYER_XP_ALL_SKILLS
        Return "All Skills"
    EndIf

    Return SKILL_NAMES[SELECTED_PLAYER_XP_SKILL]

EndFunction


Function CopySkillXpToBulk(Int sourceSkill)

    BULK_XP_BASE_MULTIPLIER = UncapperMCM.GetSkillExpBaseMultiplier(sourceSkill)
    BULK_XP_OFFSET_MULTIPLIER = UncapperMCM.GetSkillExpOffsetMultiplier(sourceSkill)

    BULK_XP_SKILL_COUNT = UncapperMCM.GetSkillExpBreakpointCount(sourceSkill, false)

    Int i = 0

    While i < BULK_XP_SKILL_COUNT
        BULK_XP_SKILL_LEVELS[i] = UncapperMCM.GetSkillExpBreakpointLevel(sourceSkill, false, i)
        BULK_XP_SKILL_BASES[i] = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(sourceSkill, false, i)
        BULK_XP_SKILL_OFFSETS[i] = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(sourceSkill, false, i)
        i += 1
    EndWhile

    BULK_XP_CHARACTER_COUNT = UncapperMCM.GetSkillExpBreakpointCount(sourceSkill, true)

    i = 0

    While i < BULK_XP_CHARACTER_COUNT
        BULK_XP_CHARACTER_LEVELS[i] = UncapperMCM.GetSkillExpBreakpointLevel(sourceSkill, true, i)
        BULK_XP_CHARACTER_BASES[i] = UncapperMCM.GetSkillExpBreakpointBaseMultiplier(sourceSkill, true, i)
        BULK_XP_CHARACTER_OFFSETS[i] = UncapperMCM.GetSkillExpBreakpointOffsetMultiplier(sourceSkill, true, i)
        i += 1
    EndWhile

EndFunction


Function CopyPlayerXpToBulk(Int sourceSkill)

    BULK_PLAYER_XP_MULTIPLIER = UncapperMCM.GetPlayerLevelExpMultiplier(sourceSkill)

    BULK_PLAYER_XP_SKILL_COUNT = UncapperMCM.GetPlayerLevelExpBreakpointCount(sourceSkill, false)

    Int i = 0

    While i < BULK_PLAYER_XP_SKILL_COUNT
        BULK_PLAYER_XP_SKILL_LEVELS[i] = UncapperMCM.GetPlayerLevelExpBreakpointLevel(sourceSkill, false, i)
        BULK_PLAYER_XP_SKILL_MULTIPLIERS[i] = UncapperMCM.GetPlayerLevelExpBreakpointMultiplier(sourceSkill, false, i)
        i += 1
    EndWhile

    BULK_PLAYER_XP_CHARACTER_COUNT = UncapperMCM.GetPlayerLevelExpBreakpointCount(sourceSkill, true)

    i = 0

    While i < BULK_PLAYER_XP_CHARACTER_COUNT
        BULK_PLAYER_XP_CHARACTER_LEVELS[i] = UncapperMCM.GetPlayerLevelExpBreakpointLevel(sourceSkill, true, i)
        BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[i] = UncapperMCM.GetPlayerLevelExpBreakpointMultiplier(sourceSkill, true, i)
        i += 1
    EndWhile

EndFunction


; ============================================================================
; Bulk Skill XP helpers
; ============================================================================

Int Function GetBulkXpCount(Bool characterLevel)

    If characterLevel
        Return BULK_XP_CHARACTER_COUNT
    EndIf

    Return BULK_XP_SKILL_COUNT

EndFunction


Int Function GetBulkXpLevel(Bool characterLevel, Int index)

    If characterLevel
        Return BULK_XP_CHARACTER_LEVELS[index]
    EndIf

    Return BULK_XP_SKILL_LEVELS[index]

EndFunction


Int Function GetBulkXpBase(Bool characterLevel, Int index)

    If characterLevel
        Return BULK_XP_CHARACTER_BASES[index]
    EndIf

    Return BULK_XP_SKILL_BASES[index]

EndFunction


Int Function GetBulkXpOffset(Bool characterLevel, Int index)

    If characterLevel
        Return BULK_XP_CHARACTER_OFFSETS[index]
    EndIf

    Return BULK_XP_SKILL_OFFSETS[index]

EndFunction


Function SetBulkXpBase(Bool characterLevel, Int index, Int value)

    If characterLevel
        BULK_XP_CHARACTER_BASES[index] = value
    Else
        BULK_XP_SKILL_BASES[index] = value
    EndIf

EndFunction


Function SetBulkXpOffset(Bool characterLevel, Int index, Int value)

    If characterLevel
        BULK_XP_CHARACTER_OFFSETS[index] = value
    Else
        BULK_XP_SKILL_OFFSETS[index] = value
    EndIf

EndFunction


Bool Function AddBulkXpBreakpoint(Bool characterLevel, Int level)

    If level <= 0 || level > 500
        Return false
    EndIf

    Int count = GetBulkXpCount(characterLevel)

    If count >= 32
        Return false
    EndIf

    Int i = 0

    While i < count
        If GetBulkXpLevel(characterLevel, i) == level
            Return false
        EndIf
        i += 1
    EndWhile

    If characterLevel
        BULK_XP_CHARACTER_LEVELS[count] = level
        BULK_XP_CHARACTER_BASES[count] = 100
        BULK_XP_CHARACTER_OFFSETS[count] = 100
        BULK_XP_CHARACTER_COUNT = count + 1
    Else
        BULK_XP_SKILL_LEVELS[count] = level
        BULK_XP_SKILL_BASES[count] = 100
        BULK_XP_SKILL_OFFSETS[count] = 100
        BULK_XP_SKILL_COUNT = count + 1
    EndIf

    SortBulkXpBreakpoints(characterLevel)

    Return true

EndFunction


Bool Function RemoveBulkXpBreakpoint(Bool characterLevel, Int index)

    Int count = GetBulkXpCount(characterLevel)

    If index <= 0 || index >= count
        Return false
    EndIf

    Int i = index

    While i < count - 1

        If characterLevel
            BULK_XP_CHARACTER_LEVELS[i] = BULK_XP_CHARACTER_LEVELS[i + 1]
            BULK_XP_CHARACTER_BASES[i] = BULK_XP_CHARACTER_BASES[i + 1]
            BULK_XP_CHARACTER_OFFSETS[i] = BULK_XP_CHARACTER_OFFSETS[i + 1]
        Else
            BULK_XP_SKILL_LEVELS[i] = BULK_XP_SKILL_LEVELS[i + 1]
            BULK_XP_SKILL_BASES[i] = BULK_XP_SKILL_BASES[i + 1]
            BULK_XP_SKILL_OFFSETS[i] = BULK_XP_SKILL_OFFSETS[i + 1]
        EndIf

        i += 1

    EndWhile

    If characterLevel
        BULK_XP_CHARACTER_COUNT = count - 1
    Else
        BULK_XP_SKILL_COUNT = count - 1
    EndIf

    Return true

EndFunction


Bool Function SetBulkXpBreakpointLevel(Bool characterLevel, Int index, Int level)

    Int count = GetBulkXpCount(characterLevel)

    If index <= 0 || index >= count || level <= 0 || level > 500
        Return false
    EndIf

    Int i = 0

    While i < count
        If i != index && GetBulkXpLevel(characterLevel, i) == level
            Return false
        EndIf
        i += 1
    EndWhile

    If characterLevel
        BULK_XP_CHARACTER_LEVELS[index] = level
    Else
        BULK_XP_SKILL_LEVELS[index] = level
    EndIf

    SortBulkXpBreakpoints(characterLevel)

    Return true

EndFunction


Function SortBulkXpBreakpoints(Bool characterLevel)

    Int count = GetBulkXpCount(characterLevel)
    Int i = 0

    While i < count - 1

        Int j = i + 1

        While j < count

            If GetBulkXpLevel(characterLevel, j) < GetBulkXpLevel(characterLevel, i)

                If characterLevel

                    Int tempLevel = BULK_XP_CHARACTER_LEVELS[i]
                    Int tempBase = BULK_XP_CHARACTER_BASES[i]
                    Int tempOffset = BULK_XP_CHARACTER_OFFSETS[i]

                    BULK_XP_CHARACTER_LEVELS[i] = BULK_XP_CHARACTER_LEVELS[j]
                    BULK_XP_CHARACTER_BASES[i] = BULK_XP_CHARACTER_BASES[j]
                    BULK_XP_CHARACTER_OFFSETS[i] = BULK_XP_CHARACTER_OFFSETS[j]

                    BULK_XP_CHARACTER_LEVELS[j] = tempLevel
                    BULK_XP_CHARACTER_BASES[j] = tempBase
                    BULK_XP_CHARACTER_OFFSETS[j] = tempOffset

                Else

                    Int tempLevel = BULK_XP_SKILL_LEVELS[i]
                    Int tempBase = BULK_XP_SKILL_BASES[i]
                    Int tempOffset = BULK_XP_SKILL_OFFSETS[i]

                    BULK_XP_SKILL_LEVELS[i] = BULK_XP_SKILL_LEVELS[j]
                    BULK_XP_SKILL_BASES[i] = BULK_XP_SKILL_BASES[j]
                    BULK_XP_SKILL_OFFSETS[i] = BULK_XP_SKILL_OFFSETS[j]

                    BULK_XP_SKILL_LEVELS[j] = tempLevel
                    BULK_XP_SKILL_BASES[j] = tempBase
                    BULK_XP_SKILL_OFFSETS[j] = tempOffset

                EndIf

            EndIf

            j += 1

        EndWhile

        i += 1

    EndWhile

EndFunction


; ============================================================================
; Bulk Player Level XP helpers
; ============================================================================

Int Function GetBulkPlayerXpCount(Bool characterLevel)

    If characterLevel
        Return BULK_PLAYER_XP_CHARACTER_COUNT
    EndIf

    Return BULK_PLAYER_XP_SKILL_COUNT

EndFunction


Int Function GetBulkPlayerXpLevel(Bool characterLevel, Int index)

    If characterLevel
        Return BULK_PLAYER_XP_CHARACTER_LEVELS[index]
    EndIf

    Return BULK_PLAYER_XP_SKILL_LEVELS[index]

EndFunction


Int Function GetBulkPlayerXpMultiplier(Bool characterLevel, Int index)

    If characterLevel
        Return BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[index]
    EndIf

    Return BULK_PLAYER_XP_SKILL_MULTIPLIERS[index]

EndFunction


Function SetBulkPlayerXpMultiplier(Bool characterLevel, Int index, Int value)

    If characterLevel
        BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[index] = value
    Else
        BULK_PLAYER_XP_SKILL_MULTIPLIERS[index] = value
    EndIf

EndFunction


Bool Function AddBulkPlayerXpBreakpoint(Bool characterLevel, Int level)

    If level <= 0 || level > 500
        Return false
    EndIf

    Int count = GetBulkPlayerXpCount(characterLevel)

    If count >= 32
        Return false
    EndIf

    Int i = 0

    While i < count
        If GetBulkPlayerXpLevel(characterLevel, i) == level
            Return false
        EndIf
        i += 1
    EndWhile

    If characterLevel
        BULK_PLAYER_XP_CHARACTER_LEVELS[count] = level
        BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[count] = 100
        BULK_PLAYER_XP_CHARACTER_COUNT = count + 1
    Else
        BULK_PLAYER_XP_SKILL_LEVELS[count] = level
        BULK_PLAYER_XP_SKILL_MULTIPLIERS[count] = 100
        BULK_PLAYER_XP_SKILL_COUNT = count + 1
    EndIf

    SortBulkPlayerXpBreakpoints(characterLevel)

    Return true

EndFunction


Bool Function RemoveBulkPlayerXpBreakpoint(Bool characterLevel, Int index)

    Int count = GetBulkPlayerXpCount(characterLevel)

    If index <= 0 || index >= count
        Return false
    EndIf

    Int i = index

    While i < count - 1

        If characterLevel
            BULK_PLAYER_XP_CHARACTER_LEVELS[i] = BULK_PLAYER_XP_CHARACTER_LEVELS[i + 1]
            BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[i] = BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[i + 1]
        Else
            BULK_PLAYER_XP_SKILL_LEVELS[i] = BULK_PLAYER_XP_SKILL_LEVELS[i + 1]
            BULK_PLAYER_XP_SKILL_MULTIPLIERS[i] = BULK_PLAYER_XP_SKILL_MULTIPLIERS[i + 1]
        EndIf

        i += 1

    EndWhile

    If characterLevel
        BULK_PLAYER_XP_CHARACTER_COUNT = count - 1
    Else
        BULK_PLAYER_XP_SKILL_COUNT = count - 1
    EndIf

    Return true

EndFunction


Bool Function SetBulkPlayerXpBreakpointLevel(Bool characterLevel, Int index, Int level)

    Int count = GetBulkPlayerXpCount(characterLevel)

    If index <= 0 || index >= count || level <= 0 || level > 500
        Return false
    EndIf

    Int i = 0

    While i < count
        If i != index && GetBulkPlayerXpLevel(characterLevel, i) == level
            Return false
        EndIf
        i += 1
    EndWhile

    If characterLevel
        BULK_PLAYER_XP_CHARACTER_LEVELS[index] = level
    Else
        BULK_PLAYER_XP_SKILL_LEVELS[index] = level
    EndIf

    SortBulkPlayerXpBreakpoints(characterLevel)

    Return true

EndFunction


Function SortBulkPlayerXpBreakpoints(Bool characterLevel)

    Int count = GetBulkPlayerXpCount(characterLevel)
    Int i = 0

    While i < count - 1

        Int j = i + 1

        While j < count

            If GetBulkPlayerXpLevel(characterLevel, j) < GetBulkPlayerXpLevel(characterLevel, i)

                If characterLevel

                    Int tempLevel = BULK_PLAYER_XP_CHARACTER_LEVELS[i]
                    Int tempMultiplier = BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[i]

                    BULK_PLAYER_XP_CHARACTER_LEVELS[i] = BULK_PLAYER_XP_CHARACTER_LEVELS[j]
                    BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[i] = BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[j]

                    BULK_PLAYER_XP_CHARACTER_LEVELS[j] = tempLevel
                    BULK_PLAYER_XP_CHARACTER_MULTIPLIERS[j] = tempMultiplier

                Else

                    Int tempLevel = BULK_PLAYER_XP_SKILL_LEVELS[i]
                    Int tempMultiplier = BULK_PLAYER_XP_SKILL_MULTIPLIERS[i]

                    BULK_PLAYER_XP_SKILL_LEVELS[i] = BULK_PLAYER_XP_SKILL_LEVELS[j]
                    BULK_PLAYER_XP_SKILL_MULTIPLIERS[i] = BULK_PLAYER_XP_SKILL_MULTIPLIERS[j]

                    BULK_PLAYER_XP_SKILL_LEVELS[j] = tempLevel
                    BULK_PLAYER_XP_SKILL_MULTIPLIERS[j] = tempMultiplier

                EndIf

            EndIf

            j += 1

        EndWhile

        i += 1

    EndWhile

EndFunction


; ============================================================================
; Bulk apply helpers
; ============================================================================

Bool Function ApplySkillXpBaseToAll()

    Int skill = 0

    While skill < 18

        If !UncapperMCM.SetSkillExpBaseMultipliers(skill, BULK_XP_BASE_MULTIPLIER, BULK_XP_OFFSET_MULTIPLIER)
            Return false
        EndIf

        skill += 1

    EndWhile

    Return true

EndFunction


Bool Function ApplyPlayerXpBaseToAll()

    Int skill = 0

    While skill < 18

        If !UncapperMCM.SetPlayerLevelExpMultiplier(skill, BULK_PLAYER_XP_MULTIPLIER)
            Return false
        EndIf

        skill += 1

    EndWhile

    Return true

EndFunction


Bool Function ApplySkillXpBreakpointsToAll(Bool characterLevel)

    Int templateCount = GetBulkXpCount(characterLevel)

    If templateCount <= 0
        Return false
    EndIf

    Int skill = 0

    While skill < 18

        Int currentCount = UncapperMCM.GetSkillExpBreakpointCount(skill, characterLevel)
        Int removeIndex = currentCount - 1

        While removeIndex >= 1

            If !UncapperMCM.RemoveSkillExpBreakpoint(skill, characterLevel, removeIndex)
                Return false
            EndIf

            removeIndex -= 1

        EndWhile

        If !UncapperMCM.SetSkillExpBreakpoint(skill, characterLevel, 0, 0, GetBulkXpBase(characterLevel, 0), GetBulkXpOffset(characterLevel, 0))
            Return false
        EndIf

        Int i = 1

        While i < templateCount

            If !UncapperMCM.AddSkillExpBreakpoint(skill, characterLevel, GetBulkXpLevel(characterLevel, i), GetBulkXpBase(characterLevel, i), GetBulkXpOffset(characterLevel, i))
                Return false
            EndIf

            i += 1

        EndWhile

        skill += 1

    EndWhile

    Return true

EndFunction


Bool Function ApplyPlayerXpBreakpointsToAll(Bool characterLevel)

    Int templateCount = GetBulkPlayerXpCount(characterLevel)

    If templateCount <= 0
        Return false
    EndIf

    Int skill = 0

    While skill < 18

        Int currentCount = UncapperMCM.GetPlayerLevelExpBreakpointCount(skill, characterLevel)
        Int removeIndex = currentCount - 1

        While removeIndex >= 1

            If !UncapperMCM.RemovePlayerLevelExpBreakpoint(skill, characterLevel, removeIndex)
                Return false
            EndIf

            removeIndex -= 1

        EndWhile

        If !UncapperMCM.SetPlayerLevelExpBreakpoint(skill, characterLevel, 0, 0, GetBulkPlayerXpMultiplier(characterLevel, 0))
            Return false
        EndIf

        Int i = 1

        While i < templateCount

            If !UncapperMCM.AddPlayerLevelExpBreakpoint(skill, characterLevel, GetBulkPlayerXpLevel(characterLevel, i), GetBulkPlayerXpMultiplier(characterLevel, i))
                Return false
            EndIf

            i += 1

        EndWhile

        skill += 1

    EndWhile

    Return true

EndFunction
