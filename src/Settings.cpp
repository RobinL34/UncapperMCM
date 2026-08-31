#include "PCH.h"

#include "Settings.h"
#include "UncapperAPI.h"

namespace
{
    Settings::Data g_settings;

    const std::vector<Settings::SkillExpBreakpoint>
        g_emptySkillExpBreakpoints;

    const std::vector<Settings::MultiplierBreakpoint>
        g_emptyMultiplierBreakpoints;

    const std::vector<Settings::AttributeBreakpoint>
        g_emptyAttributeBreakpoints;

    bool IsValidAttributeTableIndex(
        std::size_t tableIndex)
    {
        return tableIndex <
               Settings::ATTRIBUTE_TABLE_COUNT;
    }

    bool IsValidMultiplier(
        std::uint32_t value)
    {
        return value >= Settings::MIN_MULTIPLIER_HUNDREDTHS &&
               value <= Settings::MAX_MULTIPLIER_HUNDREDTHS;
    }

    bool IsValidAttributeValue(
        std::uint32_t value)
    {
        return value >= Settings::ATTRIBUTE_MIN_VALUE &&
               value <= Settings::ATTRIBUTE_MAX_VALUE;
    }

    bool ValidateSkillExpBreakpoints(
        const std::vector<Settings::SkillExpBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_SKILL_EXP_BREAKPOINTS)
        {
            return false;
        }

        if (breakpoints.front().level != 0)
        {
            return false;
        }

        std::uint32_t previousLevel = 0;

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                breakpoint.level >
                Settings::MAX_BREAKPOINT_LEVEL)
            {
                return false;
            }

            if (
                !IsValidMultiplier(
                    breakpoint.baseHundredths) ||
                !IsValidMultiplier(
                    breakpoint.offsetHundredths))
            {
                return false;
            }

            if (
                i > 0 &&
                breakpoint.level <= previousLevel)
            {
                return false;
            }

            previousLevel =
                breakpoint.level;
        }

        return true;
    }

    bool ValidateMultiplierBreakpoints(
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_LEVEL_EXP_BREAKPOINTS)
        {
            return false;
        }

        if (breakpoints.front().level != 0)
        {
            return false;
        }

        std::uint32_t previousLevel = 0;

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                breakpoint.level >
                Settings::MAX_BREAKPOINT_LEVEL)
            {
                return false;
            }

            if (
                !IsValidMultiplier(
                    breakpoint.multiplierHundredths))
            {
                return false;
            }

            if (
                i > 0 &&
                breakpoint.level <= previousLevel)
            {
                return false;
            }

            previousLevel =
                breakpoint.level;
        }

        return true;
    }

    bool LoadSkillExpBreakpointsFromIni(
        std::size_t skillSlot,
        bool characterLevel,
        std::vector<Settings::SkillExpBreakpoint> &output)
    {
        const auto slot =
            static_cast<std::uint32_t>(
                skillSlot);

        const auto count =
            characterLevel
                ? UncapperAPI::
                      GetIniSkillExpCharacterLevelBreakpointCount(
                          slot)
                : UncapperAPI::
                      GetIniSkillExpSkillLevelBreakpointCount(
                          slot);

        if (
            count == UINT32_MAX ||
            count == 0 ||
            count >
                Settings::MAX_SKILL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid Skill XP breakpoint count {} "
                "for skill slot {}.",
                count,
                skillSlot);

            return false;
        }

        output.clear();
        output.reserve(count);

        for (
            std::uint32_t index = 0;
            index < count;
            ++index)
        {
            const auto level =
                characterLevel
                    ? UncapperAPI::
                          GetIniSkillExpCharacterLevelBreakpointLevel(
                              slot,
                              index)
                    : UncapperAPI::
                          GetIniSkillExpSkillLevelBreakpointLevel(
                              slot,
                              index);

            const auto baseMultiplier =
                characterLevel
                    ? UncapperAPI::
                          GetIniSkillExpCharacterLevelBreakpointBaseMultiplier(
                              slot,
                              index)
                    : UncapperAPI::
                          GetIniSkillExpSkillLevelBreakpointBaseMultiplier(
                              slot,
                              index);

            const auto offsetMultiplier =
                characterLevel
                    ? UncapperAPI::
                          GetIniSkillExpCharacterLevelBreakpointOffsetMultiplier(
                              slot,
                              index)
                    : UncapperAPI::
                          GetIniSkillExpSkillLevelBreakpointOffsetMultiplier(
                              slot,
                              index);

            if (
                level == UINT32_MAX ||
                baseMultiplier == UINT32_MAX ||
                offsetMultiplier == UINT32_MAX)
            {
                SKSE::log::error(
                    "Failed to read Skill XP breakpoint {} "
                    "for skill slot {}.",
                    index,
                    skillSlot);

                return false;
            }

            output.push_back(
                Settings::SkillExpBreakpoint{
                    level,
                    baseMultiplier,
                    offsetMultiplier});
        }

        if (
            !ValidateSkillExpBreakpoints(
                output))
        {
            SKSE::log::error(
                "Invalid Skill XP breakpoint table "
                "loaded for skill slot {}.",
                skillSlot);

            return false;
        }

        return true;
    }

    bool ApplySkillExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel,
        const std::vector<Settings::SkillExpBreakpoint> &breakpoints)
    {
        if (
            !ValidateSkillExpBreakpoints(
                breakpoints))
        {
            SKSE::log::error(
                "Cannot apply invalid Skill XP breakpoint "
                "table for skill slot {}.",
                skillSlot);

            return false;
        }

        const auto slot =
            static_cast<std::uint32_t>(
                skillSlot);

        const bool beginResult =
            characterLevel
                ? UncapperAPI::
                      BeginSkillExpCharacterLevelOverride(
                          slot)
                : UncapperAPI::
                      BeginSkillExpSkillLevelOverride(
                          slot);

        if (!beginResult)
        {
            return false;
        }

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            const bool setResult =
                characterLevel
                    ? UncapperAPI::
                          SetSkillExpCharacterLevelBreakpoint(
                              slot,
                              static_cast<std::uint32_t>(i),
                              breakpoint.level,
                              breakpoint.baseHundredths,
                              breakpoint.offsetHundredths)
                    : UncapperAPI::
                          SetSkillExpSkillLevelBreakpoint(
                              slot,
                              static_cast<std::uint32_t>(i),
                              breakpoint.level,
                              breakpoint.baseHundredths,
                              breakpoint.offsetHundredths);

            if (!setResult)
            {
                SKSE::log::error(
                    "Failed to set Skill XP breakpoint {} "
                    "for skill slot {}.",
                    i,
                    skillSlot);

                return false;
            }
        }

        return characterLevel
                   ? UncapperAPI::
                         CommitSkillExpCharacterLevelOverride(
                             slot,
                             static_cast<std::uint32_t>(
                                 breakpoints.size()))
                   : UncapperAPI::
                         CommitSkillExpSkillLevelOverride(
                             slot,
                             static_cast<std::uint32_t>(
                                 breakpoints.size()));
    }

    bool LoadPlayerLevelExpBreakpointsFromIni(
        std::size_t skillSlot,
        bool characterLevel,
        std::vector<Settings::MultiplierBreakpoint> &output)
    {
        const auto slot =
            static_cast<std::uint32_t>(
                skillSlot);

        const auto count =
            characterLevel
                ? UncapperAPI::
                      GetIniLevelExpCharacterLevelBreakpointCount(
                          slot)
                : UncapperAPI::
                      GetIniLevelExpSkillLevelBreakpointCount(
                          slot);

        if (
            count == UINT32_MAX ||
            count == 0 ||
            count >
                Settings::MAX_LEVEL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid Player Level XP breakpoint count {} "
                "for skill slot {}.",
                count,
                skillSlot);

            return false;
        }

        output.clear();
        output.reserve(count);

        for (
            std::uint32_t index = 0;
            index < count;
            ++index)
        {
            const auto level =
                characterLevel
                    ? UncapperAPI::
                          GetIniLevelExpCharacterLevelBreakpointLevel(
                              slot,
                              index)
                    : UncapperAPI::
                          GetIniLevelExpSkillLevelBreakpointLevel(
                              slot,
                              index);

            const auto multiplier =
                characterLevel
                    ? UncapperAPI::
                          GetIniLevelExpCharacterLevelBreakpointMultiplier(
                              slot,
                              index)
                    : UncapperAPI::
                          GetIniLevelExpSkillLevelBreakpointMultiplier(
                              slot,
                              index);

            if (
                level == UINT32_MAX ||
                multiplier == UINT32_MAX)
            {
                SKSE::log::error(
                    "Failed to read Player Level XP breakpoint {} "
                    "for skill slot {}.",
                    index,
                    skillSlot);

                return false;
            }

            output.push_back(
                Settings::MultiplierBreakpoint{
                    level,
                    multiplier});
        }

        if (
            !ValidateMultiplierBreakpoints(
                output))
        {
            SKSE::log::error(
                "Invalid Player Level XP breakpoint table "
                "loaded for skill slot {}.",
                skillSlot);

            return false;
        }

        return true;
    }

    bool ApplyPlayerLevelExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel,
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            return false;
        }

        const auto slot =
            static_cast<std::uint32_t>(
                skillSlot);

        const bool beginResult =
            characterLevel
                ? UncapperAPI::
                      BeginLevelExpCharacterLevelOverride(
                          slot)
                : UncapperAPI::
                      BeginLevelExpSkillLevelOverride(
                          slot);

        if (!beginResult)
        {
            return false;
        }

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            const bool setResult =
                characterLevel
                    ? UncapperAPI::
                          SetLevelExpCharacterLevelBreakpoint(
                              slot,
                              static_cast<std::uint32_t>(i),
                              breakpoint.level,
                              breakpoint.multiplierHundredths)
                    : UncapperAPI::
                          SetLevelExpSkillLevelBreakpoint(
                              slot,
                              static_cast<std::uint32_t>(i),
                              breakpoint.level,
                              breakpoint.multiplierHundredths);

            if (!setResult)
            {
                return false;
            }
        }

        return characterLevel
                   ? UncapperAPI::
                         CommitLevelExpCharacterLevelOverride(
                             slot,
                             static_cast<std::uint32_t>(
                                 breakpoints.size()))
                   : UncapperAPI::
                         CommitLevelExpSkillLevelOverride(
                             slot,
                             static_cast<std::uint32_t>(
                                 breakpoints.size()));
    }

    bool LoadPerksAtLevelUpFromIni(
        std::vector<Settings::MultiplierBreakpoint> &output)
    {
        const auto count =
            UncapperAPI::
                GetIniPerksAtLevelUpBreakpointCount();

        if (
            count == UINT32_MAX ||
            count == 0 ||
            count >
                Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid PerksAtLevelUp breakpoint count {}.",
                count);

            return false;
        }

        std::vector<Settings::MultiplierBreakpoint>
            breakpoints;

        breakpoints.reserve(count);

        for (
            std::uint32_t index = 0;
            index < count;
            ++index)
        {
            const auto level =
                UncapperAPI::
                    GetIniPerksAtLevelUpBreakpointLevel(
                        index);

            const auto value =
                UncapperAPI::
                    GetIniPerksAtLevelUpBreakpointValue(
                        index);

            if (
                level == UINT32_MAX ||
                value == UINT32_MAX)
            {
                SKSE::log::error(
                    "Failed to read PerksAtLevelUp breakpoint {}.",
                    index);

                return false;
            }

            breakpoints.push_back(
                Settings::MultiplierBreakpoint{
                    level,
                    value});
        }

        if (
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            SKSE::log::error(
                "Invalid PerksAtLevelUp breakpoint table "
                "loaded from SkyrimUncapper.ini.");

            return false;
        }

        output =
            std::move(breakpoints);

        return true;
    }

    bool ValidateAttributeBreakpoints(
        const std::vector<Settings::AttributeBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_ATTRIBUTE_BREAKPOINTS)
        {
            return false;
        }

        if (breakpoints.front().level != 0)
        {
            return false;
        }

        std::uint32_t previousLevel = 0;

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                breakpoint.level >
                Settings::MAX_BREAKPOINT_LEVEL)
            {
                return false;
            }

            if (
                !IsValidAttributeValue(
                    breakpoint.value))
            {
                return false;
            }

            if (
                i > 0 &&
                breakpoint.level <= previousLevel)
            {
                return false;
            }

            previousLevel =
                breakpoint.level;
        }

        return true;
    }

    bool ValidateLegendaryRuntimeSettings(
        const Settings::LegendarySettings &settings)
    {
        if (
            settings.skillLevelEnable <
                Settings::MIN_LEGENDARY_SKILL_LEVEL ||
            settings.skillLevelEnable >
                Settings::MAX_LEGENDARY_SKILL_LEVEL)
        {
            return false;
        }

        if (
            settings.skillLevelAfter <
                Settings::MIN_AFTER_LEGENDARY_LEVEL ||
            settings.skillLevelAfter >
                Settings::MAX_AFTER_LEGENDARY_LEVEL)
        {
            return false;
        }

        if (
            !settings.keepSkillLevel &&
            settings.skillLevelAfter != 0 &&
            settings.skillLevelAfter >=
                settings.skillLevelEnable)
        {
            return false;
        }

        return true;
    }

    bool ApplyPerksAtLevelUpBreakpointTable(
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (
            breakpoints.size() >
                Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS ||
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            return false;
        }

        if (
            !UncapperAPI::
                BeginPerksAtLevelUpOverride())
        {
            return false;
        }

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                !UncapperAPI::
                    SetPerksAtLevelUpBreakpoint(
                        static_cast<std::uint32_t>(i),
                        breakpoint.level,
                        breakpoint.multiplierHundredths))
            {
                return false;
            }
        }

        return UncapperAPI::
            CommitPerksAtLevelUpOverride(
                static_cast<std::uint32_t>(
                    breakpoints.size()));
    }

    bool LoadAttributeBreakpointTableFromIni(
        std::size_t tableIndex,
        std::vector<Settings::AttributeBreakpoint> &output)
    {
        if (!IsValidAttributeTableIndex(tableIndex))
        {
            return false;
        }

        const auto table =
            static_cast<std::uint32_t>(
                tableIndex);

        const auto count =
            UncapperAPI::
                GetIniAttributeBreakpointCount(
                    table);

        if (
            count == UINT32_MAX ||
            count == 0 ||
            count >
                Settings::MAX_ATTRIBUTE_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid Attribute breakpoint count {} "
                "for table {}.",
                count,
                tableIndex);

            return false;
        }

        std::vector<Settings::AttributeBreakpoint>
            breakpoints;

        breakpoints.reserve(count);

        for (
            std::uint32_t index = 0;
            index < count;
            ++index)
        {
            const auto level =
                UncapperAPI::
                    GetIniAttributeBreakpointLevel(
                        table,
                        index);

            const auto value =
                UncapperAPI::
                    GetIniAttributeBreakpointValue(
                        table,
                        index);

            if (
                level == UINT32_MAX ||
                value == UINT32_MAX)
            {
                SKSE::log::error(
                    "Failed to read Attribute breakpoint {} "
                    "for table {}.",
                    index,
                    tableIndex);

                return false;
            }

            if (
                !IsValidAttributeValue(
                    value))
            {
                SKSE::log::error(
                    "Invalid Attribute value {} at breakpoint {} "
                    "for table {}.",
                    value,
                    index,
                    tableIndex);

                return false;
            }

            breakpoints.push_back(
                Settings::AttributeBreakpoint{
                    level,
                    value});
        }

        if (
            !ValidateAttributeBreakpoints(
                breakpoints))
        {
            SKSE::log::error(
                "Invalid Attribute breakpoint table {} "
                "loaded from SkyrimUncapper.ini.",
                tableIndex);

            return false;
        }

        output =
            std::move(breakpoints);

        return true;
    }

    bool LoadAttributesAtLevelUpFromIni(
        std::array<
            std::vector<Settings::AttributeBreakpoint>,
            Settings::ATTRIBUTE_TABLE_COUNT> &output)
    {
        std::array<
            std::vector<Settings::AttributeBreakpoint>,
            Settings::ATTRIBUTE_TABLE_COUNT>
            tables{};

        for (
            std::size_t tableIndex = 0;
            tableIndex < Settings::ATTRIBUTE_TABLE_COUNT;
            ++tableIndex)
        {
            if (
                !LoadAttributeBreakpointTableFromIni(
                    tableIndex,
                    tables[tableIndex]))
            {
                return false;
            }
        }

        output =
            std::move(tables);

        return true;
    }

    bool ApplyAttributeBreakpointTable(
        std::size_t tableIndex,
        const std::vector<Settings::AttributeBreakpoint> &breakpoints)
    {
        if (
            !IsValidAttributeTableIndex(tableIndex) ||
            !ValidateAttributeBreakpoints(
                breakpoints))
        {
            return false;
        }

        const auto table =
            static_cast<std::uint32_t>(
                tableIndex);

        if (
            !UncapperAPI::
                BeginAttributeOverride(
                    table))
        {
            SKSE::log::error(
                "Failed to begin Attribute override for table {}.",
                tableIndex);

            return false;
        }

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                !UncapperAPI::
                    SetAttributeBreakpoint(
                        table,
                        static_cast<std::uint32_t>(i),
                        breakpoint.level,
                        breakpoint.value))
            {
                SKSE::log::error(
                    "Failed to set Attribute breakpoint {} "
                    "for table {}.",
                    i,
                    tableIndex);

                return false;
            }
        }

        if (
            !UncapperAPI::
                CommitAttributeOverride(
                    table,
                    static_cast<std::uint32_t>(
                        breakpoints.size())))
        {
            SKSE::log::error(
                "Failed to commit Attribute override for table {}.",
                tableIndex);

            return false;
        }

        return true;
    }

    bool LoadLegendarySettingsFromIni(
        Settings::LegendarySettings &output,
        bool &useLegendarySettings)
    {
        const auto useLegendary =
            UncapperAPI::
                GetIniUseLegendarySettings();

        const auto keepSkillLevel =
            UncapperAPI::
                GetIniLegendaryKeepSkillLevel();

        const auto hideButton =
            UncapperAPI::
                GetIniHideLegendaryButton();

        const auto skillLevelEnable =
            UncapperAPI::
                GetIniSkillLevelEnableLegendary();

        const auto skillLevelAfter =
            UncapperAPI::
                GetIniSkillLevelAfterLegendary();

        if (
            useLegendary == UINT32_MAX ||
            keepSkillLevel == UINT32_MAX ||
            hideButton == UINT32_MAX ||
            skillLevelEnable == UINT32_MAX ||
            skillLevelAfter == UINT32_MAX)
        {
            SKSE::log::error(
                "Failed to load Legendary settings "
                "from SkyrimUncapper.ini.");

            return false;
        }

        if (
            useLegendary > 1 ||
            keepSkillLevel > 1 ||
            hideButton > 1)
        {
            SKSE::log::error(
                "Invalid Legendary boolean values loaded "
                "from SkyrimUncapper.ini: use={}, keep={}, hide={}.",
                useLegendary,
                keepSkillLevel,
                hideButton);

            return false;
        }

        Settings::LegendarySettings legendary{};

        legendary.keepSkillLevel =
            keepSkillLevel != 0;

        legendary.hideLegendaryButton =
            hideButton != 0;

        legendary.skillLevelEnable =
            skillLevelEnable;

        legendary.skillLevelAfter =
            skillLevelAfter;

        output = legendary;
        useLegendarySettings =
            useLegendary != 0;

        return true;
    }

}

namespace Settings
{
    Data &Get()
    {
        return g_settings;
    }

    bool LoadFromIni()
    {
        if (!UncapperAPI::IsAvailable())
        {
            SKSE::log::error(
                "LoadFromIni failed: "
                "SkyrimUncapper.dll unavailable.");

            return false;
        }

        for (
            std::size_t i = 0;
            i < SKILL_COUNT;
            ++i)
        {
            const auto slot =
                static_cast<std::uint32_t>(i);

            const auto skillCap =
                UncapperAPI::GetIniSkillCap(
                    slot);

            const auto formulaCap =
                UncapperAPI::GetIniFormulaCap(
                    slot);

            const auto skillExpBase =
                UncapperAPI::
                    GetIniSkillExpBaseMultiplier(
                        slot);

            const auto skillExpOffset =
                UncapperAPI::
                    GetIniSkillExpOffsetMultiplier(
                        slot);

            const auto playerLevelExp =
                UncapperAPI::
                    GetIniLevelExpMultiplier(
                        slot);

            if (
                skillCap == UINT32_MAX ||
                formulaCap == UINT32_MAX ||
                skillExpBase == UINT32_MAX ||
                skillExpOffset == UINT32_MAX ||
                playerLevelExp == UINT32_MAX)
            {
                SKSE::log::error(
                    "Failed to load INI settings "
                    "for skill slot {}.",
                    i);

                return false;
            }

            if (
                !IsValidMultiplier(
                    skillExpBase) ||
                !IsValidMultiplier(
                    skillExpOffset))
            {
                SKSE::log::error(
                    "Invalid Skill XP multiplier "
                    "loaded for skill slot {}.",
                    i);

                return false;
            }

            if (
                !IsValidMultiplier(
                    playerLevelExp))
            {
                SKSE::log::error(
                    "Invalid Player Level XP multiplier "
                    "loaded for skill slot {}.",
                    i);

                return false;
            }

            g_settings.skillCaps[i] =
                skillCap;

            g_settings.formulaCaps[i] =
                formulaCap;

            g_settings
                .skillExpGain[i]
                .baseHundredths =
                skillExpBase;

            g_settings
                .skillExpGain[i]
                .offsetHundredths =
                skillExpOffset;
            g_settings
                .playerLevelExp[i]
                .multiplierHundredths =
                playerLevelExp;

            if (
                !LoadSkillExpBreakpointsFromIni(
                    i,
                    false,
                    g_settings
                        .skillExpGain[i]
                        .bySkillLevel))
            {
                return false;
            }

            if (
                !LoadSkillExpBreakpointsFromIni(
                    i,
                    true,
                    g_settings
                        .skillExpGain[i]
                        .byCharacterLevel))
            {
                return false;
            }
            if (
                !LoadPlayerLevelExpBreakpointsFromIni(
                    i,
                    false,
                    g_settings
                        .playerLevelExp[i]
                        .bySkillLevel))
            {
                return false;
            }

            if (
                !LoadPlayerLevelExpBreakpointsFromIni(
                    i,
                    true,
                    g_settings
                        .playerLevelExp[i]
                        .byCharacterLevel))
            {
                return false;
            }
        }

        if (
            !LoadPerksAtLevelUpFromIni(
                g_settings.perksAtLevelUp))
        {
            return false;
        }

        const auto useAttributesAtLevelUp =
            UncapperAPI::
                GetIniUseAttributesAtLevelUp();

        if (
            useAttributesAtLevelUp == UINT32_MAX ||
            useAttributesAtLevelUp > 1)
        {
            SKSE::log::error(
                "Invalid bUseAttributesAtLevelUp value {}.",
                useAttributesAtLevelUp);

            return false;
        }

        if (
            !LoadAttributesAtLevelUpFromIni(
                g_settings.attributesAtLevelUp))
        {
            return false;
        }

        g_settings.useAttributesAtLevelUp =
            useAttributesAtLevelUp != 0;

        LegendarySettings legendary{};
        bool useLegendarySettings = false;

        if (
            !LoadLegendarySettingsFromIni(
                legendary,
                useLegendarySettings))
        {
            return false;
        }

        g_settings.legendary =
            legendary;

        g_settings.useLegendarySettings =
            useLegendarySettings;

        const auto magnitudeCap =
            UncapperAPI::
                GetIniEnchantMagnitudeCap();

        const auto chargeCap =
            UncapperAPI::
                GetIniEnchantChargeCap();

        if (
            magnitudeCap == UINT32_MAX ||
            chargeCap == UINT32_MAX)
        {
            SKSE::log::error(
                "Failed to load Enchanting settings "
                "from SkyrimUncapper.ini.");

            return false;
        }

        g_settings
            .enchanting
            .magnitudeLevelCap =
            magnitudeCap;

        g_settings
            .enchanting
            .chargeLevelCap =
            chargeCap;

        g_settings
            .enchanting
            .useLinearChargeFormula =
            UncapperAPI::
                GetIniEnchantUseLinearCharge();

        SKSE::log::info(
            "Skill caps, formula caps, Enchanting "
            "Skill XP, Player Level XP and PerksAtLevelUp "
            "and AttributesAtLevelUp and Legendary settings loaded "
            "from SkyrimUncapper.ini.");

        return true;
    }

    bool Reset()
    {
        g_settings = Data{};

        if (!UncapperAPI::ClearOverrides())
        {
            SKSE::log::error(
                "Settings reset failed to clear runtime overrides.");

            return false;
        }

        if (!LoadFromIni())
        {
            SKSE::log::error(
                "Settings reset failed "
                "to reload INI values.");

            return false;
        }

        g_settings.enabled = false;

        SKSE::log::info(
            "Settings reset to "
            "SkyrimUncapper.ini values.");

        return true;
    }

    bool IsEnabled()
    {
        return g_settings.enabled;
    }

    bool SetEnabled(
        bool enabled)
    {
        const bool oldEnabled =
            g_settings.enabled;

        g_settings.enabled =
            enabled;

        SKSE::log::info(
            "Enabled -> {}",
            enabled);

        if (Apply())
        {
            return true;
        }

        SKSE::log::error(
            "Failed to change Enabled from {} to {}.",
            oldEnabled,
            enabled);

        g_settings.enabled =
            oldEnabled;

        if (!oldEnabled && enabled)
        {
            SKSE::log::warn(
                "Attempting recovery from a failed MCM activation "
                "by clearing partially applied runtime overrides.");

            g_settings.enabled = false;

            if (!Apply())
            {
                SKSE::log::error(
                    "Recovery after failed MCM activation also failed. "
                    "Runtime override state may be partial.");
            }
            else
            {
                SKSE::log::info(
                    "Recovery after failed MCM activation succeeded. "
                    "SkyrimUncapper.ini is back in control.");
            }
        }
        else if (oldEnabled && !enabled)
        {
            SKSE::log::error(
                "MCM deactivation failed before runtime overrides could "
                "be cleared. The previous runtime state is presumed "
                "unchanged but could not be independently verified.");
        }
        else
        {
            SKSE::log::error(
                "No safe additional runtime recovery was attempted; "
                "the logical Enabled value was restored to {}.",
                oldEnabled);
        }

        return false;
    }

    // ---------------------------------------------------------------------
    // Skill Caps
    // ---------------------------------------------------------------------

    std::uint32_t GetSkillCap(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return 0;
        }

        return g_settings
            .skillCaps[skillSlot];
    }

    bool SetSkillCap(
        std::size_t skillSlot,
        std::uint32_t value)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            SKSE::log::warn(
                "SetSkillCap rejected "
                "invalid skill slot {}.",
                skillSlot);

            return false;
        }

        if (
            value < MIN_CAP ||
            value > HARD_MAX_CAP)
        {
            SKSE::log::warn(
                "SetSkillCap rejected value {} "
                "(allowed range: {}-{}).",
                value,
                MIN_CAP,
                HARD_MAX_CAP);

            return false;
        }

        if (
            value >
            RECOMMENDED_MAX_CAP)
        {
            SKSE::log::warn(
                "SetSkillCap using experimental "
                "value {} for skill slot {}.",
                value,
                skillSlot);
        }

        g_settings
            .skillCaps[skillSlot] =
            value;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::SetSkillCap(
            static_cast<std::uint32_t>(
                skillSlot),
            value);
    }

    // ---------------------------------------------------------------------
    // Formula Caps
    // ---------------------------------------------------------------------

    std::uint32_t GetFormulaCap(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return 0;
        }

        return g_settings
            .formulaCaps[skillSlot];
    }

    bool SetFormulaCap(
        std::size_t skillSlot,
        std::uint32_t value)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            SKSE::log::warn(
                "SetFormulaCap rejected "
                "invalid skill slot {}.",
                skillSlot);

            return false;
        }

        if (
            value < MIN_CAP ||
            value > HARD_MAX_CAP)
        {
            return false;
        }

        if (
            value >
            RECOMMENDED_MAX_CAP)
        {
            SKSE::log::warn(
                "SetFormulaCap using experimental "
                "value {} for skill slot {}.",
                value,
                skillSlot);
        }

        g_settings
            .formulaCaps[skillSlot] =
            value;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::SetFormulaCap(
            static_cast<std::uint32_t>(
                skillSlot),
            value);
    }

    // ---------------------------------------------------------------------
    // Enchanting
    // ---------------------------------------------------------------------

    std::uint32_t
    GetEnchantMagnitudeCap()
    {
        return g_settings
            .enchanting
            .magnitudeLevelCap;
    }

    bool SetEnchantMagnitudeCap(
        std::uint32_t value)
    {
        if (
            value < MIN_CAP ||
            value > HARD_MAX_CAP)
        {
            return false;
        }

        if (
            value >
            RECOMMENDED_MAX_CAP)
        {
            SKSE::log::warn(
                "SetEnchantMagnitudeCap "
                "using experimental value {}.",
                value);
        }

        g_settings
            .enchanting
            .magnitudeLevelCap =
            value;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::
            SetEnchantMagnitudeCap(
                value);
    }

    std::uint32_t
    GetEnchantChargeCap()
    {
        return g_settings
            .enchanting
            .chargeLevelCap;
    }

    bool SetEnchantChargeCap(
        std::uint32_t value)
    {
        if (
            value < MIN_CAP ||
            value >
                MAX_ENCHANTING_CHARGE_CAP)
        {
            return false;
        }

        g_settings
            .enchanting
            .chargeLevelCap =
            value;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::
            SetEnchantChargeCap(
                value);
    }

    bool
    GetEnchantUseLinearCharge()
    {
        return g_settings
            .enchanting
            .useLinearChargeFormula;
    }

    bool SetEnchantUseLinearCharge(
        bool enabled)
    {
        g_settings
            .enchanting
            .useLinearChargeFormula =
            enabled;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::
            SetEnchantUseLinearCharge(
                enabled);
    }

    // ---------------------------------------------------------------------
    // Skill XP
    // ---------------------------------------------------------------------

    std::uint32_t
    GetSkillExpBaseMultiplier(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return 0;
        }

        return g_settings
            .skillExpGain[skillSlot]
            .baseHundredths;
    }

    std::uint32_t
    GetSkillExpOffsetMultiplier(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return 0;
        }

        return g_settings
            .skillExpGain[skillSlot]
            .offsetHundredths;
    }

    bool SetSkillExpBaseMultipliers(
        std::size_t skillSlot,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !IsValidMultiplier(
                baseHundredths) ||
            !IsValidMultiplier(
                offsetHundredths))
        {
            return false;
        }

        auto &settings =
            g_settings
                .skillExpGain[skillSlot];

        settings.baseHundredths =
            baseHundredths;

        settings.offsetHundredths =
            offsetHundredths;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::
            SetSkillExpBaseMultiplier(
                static_cast<std::uint32_t>(
                    skillSlot),
                baseHundredths,
                offsetHundredths);
    }

    const std::vector<SkillExpBreakpoint> &
    GetSkillExpSkillLevelBreakpoints(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return g_emptySkillExpBreakpoints;
        }

        return g_settings
            .skillExpGain[skillSlot]
            .bySkillLevel;
    }

    const std::vector<SkillExpBreakpoint> &
    GetSkillExpCharacterLevelBreakpoints(
        std::size_t skillSlot)
    {
        if (
            skillSlot >= SKILL_COUNT)
        {
            return g_emptySkillExpBreakpoints;
        }

        return g_settings
            .skillExpGain[skillSlot]
            .byCharacterLevel;
    }

    bool SetSkillExpSkillLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<SkillExpBreakpoint> &breakpoints)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !ValidateSkillExpBreakpoints(
                breakpoints))
        {
            return false;
        }

        g_settings
            .skillExpGain[skillSlot]
            .bySkillLevel =
            breakpoints;

        if (!g_settings.enabled)
        {
            return true;
        }

        return ApplySkillExpBreakpointTable(
            skillSlot,
            false,
            breakpoints);
    }

    bool SetSkillExpCharacterLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<SkillExpBreakpoint> &breakpoints)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !ValidateSkillExpBreakpoints(
                breakpoints))
        {
            return false;
        }

        g_settings
            .skillExpGain[skillSlot]
            .byCharacterLevel =
            breakpoints;

        if (!g_settings.enabled)
        {
            return true;
        }

        return ApplySkillExpBreakpointTable(
            skillSlot,
            true,
            breakpoints);
    }

    // ---------------------------------------------------------------------
    // Player Level XP
    // ---------------------------------------------------------------------

    std::uint32_t GetPlayerLevelExpMultiplier(
        std::size_t skillSlot)
    {
        if (skillSlot >= SKILL_COUNT)
        {
            return 0;
        }

        return g_settings
            .playerLevelExp[skillSlot]
            .multiplierHundredths;
    }

    bool SetPlayerLevelExpMultiplier(
        std::size_t skillSlot,
        std::uint32_t multiplierHundredths)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !IsValidMultiplier(
                multiplierHundredths))
        {
            return false;
        }

        g_settings
            .playerLevelExp[skillSlot]
            .multiplierHundredths =
            multiplierHundredths;

        if (!g_settings.enabled)
        {
            return true;
        }

        return UncapperAPI::
            SetLevelExpMultiplier(
                static_cast<std::uint32_t>(
                    skillSlot),
                multiplierHundredths);
    }

    const std::vector<MultiplierBreakpoint> &
    GetPlayerLevelExpSkillLevelBreakpoints(
        std::size_t skillSlot)
    {
        if (skillSlot >= SKILL_COUNT)
        {
            return g_emptyMultiplierBreakpoints;
        }

        return g_settings
            .playerLevelExp[skillSlot]
            .bySkillLevel;
    }

    const std::vector<MultiplierBreakpoint> &
    GetPlayerLevelExpCharacterLevelBreakpoints(
        std::size_t skillSlot)
    {
        if (skillSlot >= SKILL_COUNT)
        {
            return g_emptyMultiplierBreakpoints;
        }

        return g_settings
            .playerLevelExp[skillSlot]
            .byCharacterLevel;
    }

    bool SetPlayerLevelExpSkillLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<MultiplierBreakpoint> &breakpoints)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            return false;
        }

        g_settings
            .playerLevelExp[skillSlot]
            .bySkillLevel =
            breakpoints;

        if (!g_settings.enabled)
        {
            return true;
        }

        return ApplyPlayerLevelExpBreakpointTable(
            skillSlot,
            false,
            breakpoints);
    }

    bool SetPlayerLevelExpCharacterLevelBreakpoints(
        std::size_t skillSlot,
        const std::vector<MultiplierBreakpoint> &breakpoints)
    {
        if (
            skillSlot >= SKILL_COUNT ||
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            return false;
        }

        g_settings
            .playerLevelExp[skillSlot]
            .byCharacterLevel =
            breakpoints;

        if (!g_settings.enabled)
        {
            return true;
        }

        return ApplyPlayerLevelExpBreakpointTable(
            skillSlot,
            true,
            breakpoints);
    }

    // ---------------------------------------------------------------------
    // Perks at level up
    // ---------------------------------------------------------------------

    const std::vector<MultiplierBreakpoint> &
    GetPerksAtLevelUpBreakpoints()
    {
        return g_settings
            .perksAtLevelUp;
    }

    bool SetPerksAtLevelUpBreakpoints(
        const std::vector<MultiplierBreakpoint> &breakpoints)
    {
        if (
            breakpoints.size() >
                MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS ||
            !ValidateMultiplierBreakpoints(
                breakpoints))
        {
            return false;
        }

        g_settings.perksAtLevelUp =
            breakpoints;

        if (!g_settings.enabled)
        {
            return true;
        }

        return ApplyPerksAtLevelUpBreakpointTable(
            breakpoints);
    }

    // ---------------------------------------------------------------------
    // Attributes at level up
    // ---------------------------------------------------------------------

    bool GetUseAttributesAtLevelUp()
    {
        return g_settings
            .useAttributesAtLevelUp;
    }

    const std::vector<AttributeBreakpoint> &
    GetAttributeBreakpoints(
        std::size_t tableIndex)
    {
        if (!IsValidAttributeTableIndex(tableIndex))
        {
            return g_emptyAttributeBreakpoints;
        }

        return g_settings
            .attributesAtLevelUp[tableIndex];
    }

    bool SetAttributeBreakpoints(
        std::size_t tableIndex,
        const std::vector<AttributeBreakpoint> &breakpoints)
    {
        if (
            !IsValidAttributeTableIndex(tableIndex) ||
            !ValidateAttributeBreakpoints(
                breakpoints) ||
            !g_settings.useAttributesAtLevelUp)
        {
            return false;
        }

        if (g_settings.enabled)
        {
            if (
                !ApplyAttributeBreakpointTable(
                    tableIndex,
                    breakpoints))
            {
                return false;
            }
        }

        g_settings
            .attributesAtLevelUp[tableIndex] =
            breakpoints;

        return true;
    }

    // ---------------------------------------------------------------------
    // Legendary settings
    // ---------------------------------------------------------------------

    bool GetUseLegendarySettings()
    {
        return g_settings
            .useLegendarySettings;
    }

    const LegendarySettings &
    GetLegendarySettings()
    {
        return g_settings
            .legendary;
    }

    bool SetLegendarySettings(
        const LegendarySettings &settings)
    {
        if (
            !g_settings.useLegendarySettings ||
            !ValidateLegendaryRuntimeSettings(
                settings))
        {
            return false;
        }

        if (g_settings.enabled)
        {
            if (
                !UncapperAPI::
                    SetLegendaryOverrides(
                        settings.keepSkillLevel,
                        settings.hideLegendaryButton,
                        settings.skillLevelEnable,
                        settings.skillLevelAfter))
            {
                return false;
            }
        }

        g_settings.legendary =
            settings;

        return true;
    }

    // ---------------------------------------------------------------------
    // Apply
    // ---------------------------------------------------------------------

    bool Apply()
    {
        if (
            !UncapperAPI::IsAvailable())
        {
            SKSE::log::error(
                "Cannot apply settings: "
                "SkyrimUncapper.dll unavailable.");

            return false;
        }

        if (!g_settings.enabled)
        {
            const bool result =
                UncapperAPI::ClearOverrides();

            SKSE::log::info(
                "MCM disabled: runtime overrides "
                "cleared. SkyrimUncapper.ini "
                "remains in control.");

            return result;
        }

        if (
            g_settings.perksAtLevelUp.size() >
                MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS ||
            !ValidateMultiplierBreakpoints(
                g_settings.perksAtLevelUp))
        {
            return false;
        }

        for (
            const auto &breakpoints :
            g_settings.attributesAtLevelUp)
        {
            if (
                !ValidateAttributeBreakpoints(
                    breakpoints))
            {
                return false;
            }
        }

        for (
            std::size_t i = 0;
            i < SKILL_COUNT;
            ++i)
        {
            const auto skillCap =
                g_settings
                    .skillCaps[i];

            const auto formulaCap =
                g_settings
                    .formulaCaps[i];

            const auto &skillExp =
                g_settings
                    .skillExpGain[i];

            const auto &playerLevelExp =
                g_settings
                    .playerLevelExp[i];

            if (
                skillCap < MIN_CAP ||
                skillCap > HARD_MAX_CAP)
            {
                return false;
            }

            if (
                formulaCap < MIN_CAP ||
                formulaCap > HARD_MAX_CAP)
            {
                return false;
            }

            if (
                !IsValidMultiplier(
                    skillExp.baseHundredths) ||
                !IsValidMultiplier(
                    skillExp.offsetHundredths))
            {
                return false;
            }

            if (
                !ValidateSkillExpBreakpoints(
                    skillExp.bySkillLevel) ||
                !ValidateSkillExpBreakpoints(
                    skillExp.byCharacterLevel))
            {
                return false;
            }

            if (
                !IsValidMultiplier(
                    playerLevelExp
                        .multiplierHundredths) ||
                !ValidateMultiplierBreakpoints(
                    playerLevelExp
                        .bySkillLevel) ||
                !ValidateMultiplierBreakpoints(
                    playerLevelExp
                        .byCharacterLevel))
            {
                return false;
            }

            if (
                !UncapperAPI::SetSkillCap(
                    static_cast<std::uint32_t>(i),
                    skillCap))
            {
                return false;
            }

            if (
                !UncapperAPI::SetFormulaCap(
                    static_cast<std::uint32_t>(i),
                    formulaCap))
            {
                return false;
            }

            if (
                !UncapperAPI::
                    SetSkillExpBaseMultiplier(
                        static_cast<std::uint32_t>(i),
                        skillExp.baseHundredths,
                        skillExp.offsetHundredths))
            {
                SKSE::log::error(
                    "Failed to apply Skill XP base "
                    "multipliers for skill slot {}.",
                    i);

                return false;
            }

            if (
                !ApplySkillExpBreakpointTable(
                    i,
                    false,
                    skillExp.bySkillLevel))
            {
                SKSE::log::error(
                    "Failed to apply Skill XP "
                    "skill-level breakpoints "
                    "for slot {}.",
                    i);

                return false;
            }

            if (
                !ApplySkillExpBreakpointTable(
                    i,
                    true,
                    skillExp.byCharacterLevel))
            {
                SKSE::log::error(
                    "Failed to apply Skill XP "
                    "character-level breakpoints "
                    "for slot {}.",
                    i);

                return false;
            }
            if (
                !UncapperAPI::
                    SetLevelExpMultiplier(
                        static_cast<std::uint32_t>(i),
                        playerLevelExp
                            .multiplierHundredths))
            {
                SKSE::log::error(
                    "Failed to apply Player Level XP "
                    "multiplier for skill slot {}.",
                    i);

                return false;
            }

            if (
                !ApplyPlayerLevelExpBreakpointTable(
                    i,
                    false,
                    playerLevelExp
                        .bySkillLevel))
            {
                SKSE::log::error(
                    "Failed to apply Player Level XP "
                    "skill-level breakpoints for slot {}.",
                    i);

                return false;
            }

            if (
                !ApplyPlayerLevelExpBreakpointTable(
                    i,
                    true,
                    playerLevelExp
                        .byCharacterLevel))
            {
                SKSE::log::error(
                    "Failed to apply Player Level XP "
                    "character-level breakpoints for slot {}.",
                    i);

                return false;
            }
        }

        if (
            !ApplyPerksAtLevelUpBreakpointTable(
                g_settings.perksAtLevelUp))
        {
            SKSE::log::error(
                "Failed to apply PerksAtLevelUp breakpoints.");

            return false;
        }

        if (g_settings.useAttributesAtLevelUp)
        {
            for (
                std::size_t tableIndex = 0;
                tableIndex < ATTRIBUTE_TABLE_COUNT;
                ++tableIndex)
            {
                if (
                    !ApplyAttributeBreakpointTable(
                        tableIndex,
                        g_settings
                            .attributesAtLevelUp[tableIndex]))
                {
                    SKSE::log::error(
                        "Failed to apply Attribute breakpoint "
                        "table {}.",
                        tableIndex);

                    return false;
                }
            }
        }

        if (g_settings.useLegendarySettings)
        {
            const auto &legendary =
                g_settings.legendary;

            if (
                ValidateLegendaryRuntimeSettings(
                    legendary))
            {
                if (
                    !UncapperAPI::
                        SetLegendaryOverrides(
                            legendary.keepSkillLevel,
                            legendary.hideLegendaryButton,
                            legendary.skillLevelEnable,
                            legendary.skillLevelAfter))
                {
                    SKSE::log::error(
                        "Failed to apply Legendary settings.");

                    return false;
                }
            }
            else
            {
                SKSE::log::warn(
                    "Legendary settings were not overridden because "
                    "the current INI-compatible values are outside "
                    "the MCM runtime limits: keep={}, hide={}, "
                    "threshold={}, after={}.",
                    legendary.keepSkillLevel,
                    legendary.hideLegendaryButton,
                    legendary.skillLevelEnable,
                    legendary.skillLevelAfter);
            }
        }

        const auto magnitudeCap =
            g_settings
                .enchanting
                .magnitudeLevelCap;

        const auto chargeCap =
            g_settings
                .enchanting
                .chargeLevelCap;

        if (
            magnitudeCap < MIN_CAP ||
            magnitudeCap > HARD_MAX_CAP)
        {
            return false;
        }

        if (
            chargeCap < MIN_CAP ||
            chargeCap >
                MAX_ENCHANTING_CHARGE_CAP)
        {
            return false;
        }

        if (
            !UncapperAPI::
                SetEnchantMagnitudeCap(
                    magnitudeCap))
        {
            return false;
        }

        if (
            !UncapperAPI::
                SetEnchantChargeCap(
                    chargeCap))
        {
            return false;
        }

        if (
            !UncapperAPI::
                SetEnchantUseLinearCharge(
                    g_settings
                        .enchanting
                        .useLinearChargeFormula))
        {
            return false;
        }

        SKSE::log::info(
            "Skill caps, formula caps, Enchanting, "
            "Skill XP, Player Level XP and PerksAtLevelUp "
            "and AttributesAtLevelUp and Legendary settings applied.");

        return true;
    }
}
