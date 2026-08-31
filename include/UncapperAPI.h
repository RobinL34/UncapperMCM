#pragma once

#include <cstdint>

namespace UncapperAPI
{
    bool IsAvailable();

    bool SetSkillCap(
        std::uint32_t skillSlot,
        std::uint32_t value
    );

    bool SetFormulaCap(
        std::uint32_t skillSlot,
        std::uint32_t value
    );

    bool SetEnchantMagnitudeCap(
        std::uint32_t value
    );

    bool SetEnchantChargeCap(
        std::uint32_t value
    );

    bool SetEnchantUseLinearCharge(
        bool enabled
    );

    bool ClearOverrides();

    std::uint32_t GetIniSkillCap(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniFormulaCap(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniEnchantMagnitudeCap();

    std::uint32_t GetIniEnchantChargeCap();

    bool GetIniEnchantUseLinearCharge();


    // ---------------------------------------------------------------------
    // Skill XP - base multipliers
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpBaseMultiplier(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniSkillExpOffsetMultiplier(
        std::uint32_t skillSlot
    );

    bool SetSkillExpBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths
    );


    // ---------------------------------------------------------------------
    // Skill XP - breakpoints by base skill level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpSkillLevelBreakpointCount(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniSkillExpSkillLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniSkillExpSkillLevelBreakpointBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniSkillExpSkillLevelBreakpointOffsetMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    bool BeginSkillExpSkillLevelOverride(
        std::uint32_t skillSlot
    );

    bool SetSkillExpSkillLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths
    );

    bool CommitSkillExpSkillLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count
    );


    // ---------------------------------------------------------------------
    // Skill XP - breakpoints by character level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointCount(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointOffsetMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    bool BeginSkillExpCharacterLevelOverride(
        std::uint32_t skillSlot
    );

    bool SetSkillExpCharacterLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths
    );

    bool CommitSkillExpCharacterLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count
    );


    // ---------------------------------------------------------------------
    // Player Level XP - base multiplier
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpMultiplier(
        std::uint32_t skillSlot
    );

    bool SetLevelExpMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t multiplierHundredths
    );


    // ---------------------------------------------------------------------
    // Player Level XP - breakpoints by base skill level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpSkillLevelBreakpointCount(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniLevelExpSkillLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniLevelExpSkillLevelBreakpointMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    bool BeginLevelExpSkillLevelOverride(
        std::uint32_t skillSlot
    );

    bool SetLevelExpSkillLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t multiplierHundredths
    );

    bool CommitLevelExpSkillLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count
    );


    // ---------------------------------------------------------------------
    // Player Level XP - breakpoints by character level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointCount(
        std::uint32_t skillSlot
    );

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index
    );

    bool BeginLevelExpCharacterLevelOverride(
        std::uint32_t skillSlot
    );

    bool SetLevelExpCharacterLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t multiplierHundredths
    );

    bool CommitLevelExpCharacterLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count
    );


    // ---------------------------------------------------------------------
    // Perks at level up
    // ---------------------------------------------------------------------

    std::uint32_t GetIniPerksAtLevelUpBreakpointCount();

    std::uint32_t GetIniPerksAtLevelUpBreakpointLevel(
        std::uint32_t index
    );

    std::uint32_t GetIniPerksAtLevelUpBreakpointValue(
        std::uint32_t index
    );

    bool BeginPerksAtLevelUpOverride();

    bool SetPerksAtLevelUpBreakpoint(
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t perkHundredths
    );

    bool CommitPerksAtLevelUpOverride(
        std::uint32_t count
    );


    // ---------------------------------------------------------------------
    // Attributes at level up
    // ---------------------------------------------------------------------

    std::uint32_t GetIniAttributeBreakpointCount(
        std::uint32_t tableIndex
    );

    std::uint32_t GetIniAttributeBreakpointLevel(
        std::uint32_t tableIndex,
        std::uint32_t index
    );

    std::uint32_t GetIniAttributeBreakpointValue(
        std::uint32_t tableIndex,
        std::uint32_t index
    );

    bool BeginAttributeOverride(
        std::uint32_t tableIndex
    );

    bool SetAttributeBreakpoint(
        std::uint32_t tableIndex,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t value
    );

    bool CommitAttributeOverride(
        std::uint32_t tableIndex,
        std::uint32_t count
    );

    std::uint32_t GetIniUseAttributesAtLevelUp();


    // ---------------------------------------------------------------------
    // Legendary settings
    // ---------------------------------------------------------------------

    std::uint32_t GetIniUseLegendarySettings();

    std::uint32_t GetIniLegendaryKeepSkillLevel();

    std::uint32_t GetIniHideLegendaryButton();

    std::uint32_t GetIniSkillLevelEnableLegendary();

    std::uint32_t GetIniSkillLevelAfterLegendary();

    bool SetLegendaryOverrides(
        bool keepSkillLevel,
        bool hideButton,
        std::uint32_t skillLevelEnable,
        std::uint32_t skillLevelAfter
    );
}
