#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Settings
{
    constexpr std::size_t SKILL_COUNT = 18;

    constexpr std::uint32_t MIN_CAP = 1;
    constexpr std::uint32_t RECOMMENDED_MAX_CAP = 400;
    constexpr std::uint32_t HARD_MAX_CAP = 500;

    constexpr std::uint32_t MAX_ENCHANTING_CHARGE_CAP = 199;

    constexpr std::uint32_t MIN_MULTIPLIER_HUNDREDTHS = 0;
    constexpr std::uint32_t MAX_MULTIPLIER_HUNDREDTHS = 10000;

    constexpr std::uint32_t MIN_BREAKPOINT_LEVEL = 0;
    constexpr std::uint32_t MAX_BREAKPOINT_LEVEL = 500;

    constexpr std::size_t MAX_SKILL_EXP_BREAKPOINTS = 32;
    constexpr std::size_t MAX_LEVEL_EXP_BREAKPOINTS = 32;


    // ---------------------------------------------------------------------
    // Generic single-value multiplier breakpoint.
    //
    // This will be useful later for LevelSkillExpMults, perks, etc.
    // ---------------------------------------------------------------------

    struct MultiplierBreakpoint
    {
        std::uint32_t level = 0;
        std::uint32_t multiplierHundredths = 100;
    };


    // ---------------------------------------------------------------------
    // Skill XP breakpoint.
    //
    // SkillExpGainMults uses TWO separate multipliers:
    // base and offset.
    // ---------------------------------------------------------------------

    struct SkillExpBreakpoint
    {
        std::uint32_t level = 0;

        std::uint32_t baseHundredths = 100;
        std::uint32_t offsetHundredths = 100;
    };


    struct IntBreakpoint
    {
        std::uint32_t level = 0;
        std::int32_t value = 0;
    };


    // ---------------------------------------------------------------------
    // Skill experience settings.
    // ---------------------------------------------------------------------

    struct SkillExpSettings
    {
        std::uint32_t baseHundredths = 100;
        std::uint32_t offsetHundredths = 100;

        std::vector<SkillExpBreakpoint> bySkillLevel;
        std::vector<SkillExpBreakpoint> byCharacterLevel;
    };


    // ---------------------------------------------------------------------
    // Player-level XP settings.
    //
    // LevelSkillExpMults only uses one multiplier, unlike SkillExpGainMults.
    // This is reserved for the next development block.
    // ---------------------------------------------------------------------

    struct PlayerLevelExpSettings
    {
        std::uint32_t multiplierHundredths = 100;

        std::vector<MultiplierBreakpoint> bySkillLevel;
        std::vector<MultiplierBreakpoint> byCharacterLevel;
    };


    struct EnchantingSettings
    {
        std::uint32_t magnitudeLevelCap = 0;
        std::uint32_t chargeLevelCap = 0;

        bool useLinearChargeFormula = false;
    };


    struct LegendarySettings
    {
        bool keepSkillLevel = false;
        bool hideLegendaryButton = false;

        std::uint32_t enableLevel = 0;
        std::uint32_t levelAfterLegendary = 0;
    };


    struct Data
    {
        bool enabled = false;

        std::array<std::uint32_t, SKILL_COUNT> skillCaps{};
        std::array<std::uint32_t, SKILL_COUNT> formulaCaps{};

        std::array<SkillExpSettings, SKILL_COUNT>
            skillExpGain{};

        std::array<PlayerLevelExpSettings, SKILL_COUNT>
            playerLevelExp{};

        EnchantingSettings enchanting{};

        std::vector<MultiplierBreakpoint> perksAtLevelUp;

        std::vector<IntBreakpoint> healthAtLevelUp;
        std::vector<IntBreakpoint> healthAtMagickaLevelUp;
        std::vector<IntBreakpoint> healthAtStaminaLevelUp;

        std::vector<IntBreakpoint> magickaAtLevelUp;
        std::vector<IntBreakpoint> magickaAtHealthLevelUp;
        std::vector<IntBreakpoint> magickaAtStaminaLevelUp;

        std::vector<IntBreakpoint> staminaAtLevelUp;
        std::vector<IntBreakpoint> staminaAtHealthLevelUp;
        std::vector<IntBreakpoint> staminaAtMagickaLevelUp;

        std::vector<IntBreakpoint> carryWeightAtHealthLevelUp;
        std::vector<IntBreakpoint> carryWeightAtMagickaLevelUp;
        std::vector<IntBreakpoint> carryWeightAtStaminaLevelUp;

        LegendarySettings legendary{};
    };


    Data& Get();

    void Reset();
    bool LoadFromIni();

    bool IsEnabled();
    void SetEnabled(bool enabled);


    // ---------------------------------------------------------------------
    // Skill caps
    // ---------------------------------------------------------------------

    std::uint32_t GetSkillCap(
        std::size_t skillSlot
    );

    bool SetSkillCap(
        std::size_t skillSlot,
        std::uint32_t value
    );


    // ---------------------------------------------------------------------
    // Formula caps
    // ---------------------------------------------------------------------

    std::uint32_t GetFormulaCap(
        std::size_t skillSlot
    );

    bool SetFormulaCap(
        std::size_t skillSlot,
        std::uint32_t value
    );


    // ---------------------------------------------------------------------
    // Enchanting
    // ---------------------------------------------------------------------

    std::uint32_t GetEnchantMagnitudeCap();

    bool SetEnchantMagnitudeCap(
        std::uint32_t value
    );

    std::uint32_t GetEnchantChargeCap();

    bool SetEnchantChargeCap(
        std::uint32_t value
    );

    bool GetEnchantUseLinearCharge();

    bool SetEnchantUseLinearCharge(
        bool enabled
    );


    // ---------------------------------------------------------------------
    // Skill XP
    // ---------------------------------------------------------------------

    std::uint32_t GetSkillExpBaseMultiplier(
        std::size_t skillSlot
    );

    std::uint32_t GetSkillExpOffsetMultiplier(
        std::size_t skillSlot
    );

    bool SetSkillExpBaseMultipliers(
        std::size_t skillSlot,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths
    );

    const std::vector<SkillExpBreakpoint>&
        GetSkillExpSkillLevelBreakpoints(
            std::size_t skillSlot
        );

    const std::vector<SkillExpBreakpoint>&
        GetSkillExpCharacterLevelBreakpoints(
            std::size_t skillSlot
        );

    bool SetSkillExpSkillLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<SkillExpBreakpoint>& breakpoints
    );

    bool SetSkillExpCharacterLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<SkillExpBreakpoint>& breakpoints
    );

        // ---------------------------------------------------------------------
    // Player Level XP
    // ---------------------------------------------------------------------

    std::uint32_t GetPlayerLevelExpMultiplier(
        std::size_t skillSlot
    );

    bool SetPlayerLevelExpMultiplier(
        std::size_t skillSlot,
        std::uint32_t multiplierHundredths
    );

    const std::vector<MultiplierBreakpoint>&
        GetPlayerLevelExpSkillLevelBreakpoints(
            std::size_t skillSlot
        );

    const std::vector<MultiplierBreakpoint>&
        GetPlayerLevelExpCharacterLevelBreakpoints(
            std::size_t skillSlot
        );

    bool SetPlayerLevelExpSkillLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<MultiplierBreakpoint>& breakpoints
    );

    bool SetPlayerLevelExpCharacterLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<MultiplierBreakpoint>& breakpoints
    );


    // ---------------------------------------------------------------------
    // Apply all runtime settings
    // ---------------------------------------------------------------------

    bool Apply();
}