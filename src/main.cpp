#include "PCH.h"
#include "UncapperAPI.h"
#include "Settings.h"
#include "Serialization.h"
#include "Papyrus/Bindings.h"
#include "Papyrus/Common.h"

#include <algorithm>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    constexpr auto PAPYRUS_CLASS =
        "UncapperMCM";

    // =========================================================================
    // Helpers
    // =========================================================================

    bool IsValidAttributeTableIndex(
        std::int32_t tableIndex)
    {
        return tableIndex >= 0 &&
               tableIndex <
                   static_cast<std::int32_t>(
                       Settings::ATTRIBUTE_TABLE_COUNT);
    }

    bool IsValidAttributeValue(
        std::int32_t value)
    {
        return value >= static_cast<std::int32_t>(
                            Settings::ATTRIBUTE_MIN_VALUE) &&
               value <= static_cast<std::int32_t>(
                            Settings::ATTRIBUTE_MAX_VALUE);
    }

    void SortAttributeBreakpoints(
        std::vector<Settings::AttributeBreakpoint> &breakpoints)
    {
        std::sort(
            breakpoints.begin(),
            breakpoints.end(),
            [](
                const Settings::AttributeBreakpoint &a,
                const Settings::AttributeBreakpoint &b)
            {
                return a.level < b.level;
            });
    }

    bool ContainsDuplicateAttributeBreakpointLevels(
        const std::vector<Settings::AttributeBreakpoint> &breakpoints)
    {
        if (breakpoints.size() < 2)
        {
            return false;
        }

        for (
            std::size_t i = 1;
            i < breakpoints.size();
            ++i)
        {
            if (
                breakpoints[i - 1].level ==
                breakpoints[i].level)
            {
                return true;
            }
        }

        return false;
    }

    // =========================================================================
    // Logging
    // =========================================================================

    void SetupLog()
    {
        auto logDir =
            SKSE::log::log_directory();

        if (!logDir)
        {
            SKSE::stl::report_and_fail(
                "Unable to find SKSE log directory.");
        }

        auto logPath =
            *logDir / "UncapperMCM.log";

        auto sink =
            std::make_shared<
                spdlog::sinks::basic_file_sink_mt>(
                logPath.string(),
                true);

        auto logger =
            std::make_shared<spdlog::logger>(
                "UncapperMCM",
                std::move(sink));

        logger->set_level(
            spdlog::level::info);

        logger->flush_on(
            spdlog::level::info);

        spdlog::set_default_logger(
            std::move(logger));

        spdlog::set_pattern(
            "[%H:%M:%S] [%l] %v");
    }

    // =========================================================================
    // Perks at level up - breakpoint getters
    // =========================================================================

    std::int32_t PapyrusGetPerksAtLevelUpBreakpointCount(
        RE::StaticFunctionTag *)
    {
        return static_cast<std::int32_t>(
            Settings::
                GetPerksAtLevelUpBreakpoints()
                .size());
    }

    std::int32_t PapyrusGetPerksAtLevelUpBreakpointLevel(
        RE::StaticFunctionTag *,
        std::int32_t index)
    {
        if (index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            Settings::
                GetPerksAtLevelUpBreakpoints();

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            breakpoints[breakpointIndex]
                .level);
    }

    std::int32_t PapyrusGetPerksAtLevelUpBreakpointValue(
        RE::StaticFunctionTag *,
        std::int32_t index)
    {
        if (index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            Settings::
                GetPerksAtLevelUpBreakpoints();

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            breakpoints[breakpointIndex]
                .multiplierHundredths);
    }

    // =========================================================================
    // Perks at level up - modify breakpoint
    // =========================================================================

    bool PapyrusSetPerksAtLevelUpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t index,
        std::int32_t level,
        std::int32_t valueHundredths)
    {
        if (
            index < 0 ||
            !Papyrus::IsValidBreakpointLevel(level) ||
            !Papyrus::IsValidMultiplier(
                valueHundredths))
        {
            return false;
        }

        auto breakpoints =
            Settings::
                GetPerksAtLevelUpBreakpoints();

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
                breakpoints.size() ||
            breakpoints.size() >
                Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS)
        {
            return false;
        }

        if (
            breakpointIndex == 0 &&
            level != 0)
        {
            SKSE::log::warn(
                "SetPerksAtLevelUpBreakpoint rejected: "
                "level 0 breakpoint cannot be moved.");

            return false;
        }

        breakpoints[breakpointIndex] =
            Settings::MultiplierBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    valueHundredths)};

        Papyrus::SortMultiplierBreakpoints(
            breakpoints);

        if (
            Papyrus::ContainsDuplicateMultiplierBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "SetPerksAtLevelUpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            Settings::
                SetPerksAtLevelUpBreakpoints(
                    breakpoints);

        SKSE::log::info(
            "Papyrus SetPerksAtLevelUpBreakpoint("
            "{}, {}, {}) -> {}",
            index,
            level,
            valueHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Perks at level up - add breakpoint
    // =========================================================================

    bool PapyrusAddPerksAtLevelUpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t level,
        std::int32_t valueHundredths)
    {
        if (
            !Papyrus::IsValidBreakpointLevel(level) ||
            !Papyrus::IsValidMultiplier(
                valueHundredths))
        {
            return false;
        }

        auto breakpoints =
            Settings::
                GetPerksAtLevelUpBreakpoints();

        if (
            breakpoints.size() >=
            Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS)
        {
            SKSE::log::warn(
                "AddPerksAtLevelUpBreakpoint rejected: "
                "maximum breakpoint count reached.");

            return false;
        }

        breakpoints.push_back(
            Settings::MultiplierBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    valueHundredths)});

        Papyrus::SortMultiplierBreakpoints(
            breakpoints);

        if (
            Papyrus::ContainsDuplicateMultiplierBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "AddPerksAtLevelUpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            Settings::
                SetPerksAtLevelUpBreakpoints(
                    breakpoints);

        SKSE::log::info(
            "Papyrus AddPerksAtLevelUpBreakpoint("
            "{}, {}) -> {}",
            level,
            valueHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Perks at level up - remove breakpoint
    // =========================================================================

    bool PapyrusRemovePerksAtLevelUpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t index)
    {
        if (index < 0)
        {
            return false;
        }

        auto breakpoints =
            Settings::
                GetPerksAtLevelUpBreakpoints();

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return false;
        }

        if (
            breakpoints[breakpointIndex].level == 0)
        {
            SKSE::log::warn(
                "RemovePerksAtLevelUpBreakpoint rejected: "
                "level 0 breakpoint cannot be removed.");

            return false;
        }

        if (breakpoints.size() <= 1)
        {
            return false;
        }

        breakpoints.erase(
            breakpoints.begin() +
            static_cast<std::ptrdiff_t>(
                breakpointIndex));

        const bool result =
            Settings::
                SetPerksAtLevelUpBreakpoints(
                    breakpoints);

        SKSE::log::info(
            "Papyrus RemovePerksAtLevelUpBreakpoint("
            "{}) -> {}",
            index,
            result);

        return result;
    }

    // =========================================================================
    // Attributes at level up - breakpoint getters
    // =========================================================================

    std::int32_t PapyrusGetAttributeBreakpointCount(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex)
    {
        if (!IsValidAttributeTableIndex(tableIndex))
        {
            return -1;
        }

        const auto &breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    static_cast<std::size_t>(
                        tableIndex));

        return static_cast<std::int32_t>(
            breakpoints.size());
    }

    std::int32_t PapyrusGetAttributeBreakpointLevel(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex,
        std::int32_t index)
    {
        if (
            !IsValidAttributeTableIndex(tableIndex) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    static_cast<std::size_t>(
                        tableIndex));

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            breakpoints[breakpointIndex]
                .level);
    }

    std::int32_t PapyrusGetAttributeBreakpointValue(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex,
        std::int32_t index)
    {
        if (
            !IsValidAttributeTableIndex(tableIndex) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    static_cast<std::size_t>(
                        tableIndex));

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            breakpoints[breakpointIndex]
                .value);
    }

    // =========================================================================
    // Attributes at level up - modify breakpoint
    // =========================================================================

    bool PapyrusSetAttributeBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex,
        std::int32_t index,
        std::int32_t level,
        std::int32_t value)
    {
        if (
            !Settings::GetUseAttributesAtLevelUp() ||
            !IsValidAttributeTableIndex(tableIndex) ||
            index < 0 ||
            !Papyrus::IsValidBreakpointLevel(level) ||
            !IsValidAttributeValue(value))
        {
            return false;
        }

        const auto table =
            static_cast<std::size_t>(
                tableIndex);

        auto breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    table);

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
            breakpoints.size())
        {
            return false;
        }

        if (
            breakpointIndex == 0 &&
            level != 0)
        {
            SKSE::log::warn(
                "SetAttributeBreakpoint rejected: "
                "level 0 breakpoint cannot be moved.");

            return false;
        }

        breakpoints[breakpointIndex] =
            Settings::AttributeBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    value)};

        SortAttributeBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateAttributeBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "SetAttributeBreakpoint rejected "
                "duplicate level {} for table {}.",
                level,
                tableIndex);

            return false;
        }

        const bool result =
            Settings::
                SetAttributeBreakpoints(
                    table,
                    breakpoints);

        SKSE::log::info(
            "Papyrus SetAttributeBreakpoint("
            "{}, {}, {}, {}) -> {}",
            tableIndex,
            index,
            level,
            value,
            result);

        return result;
    }

    // =========================================================================
    // Attributes at level up - add breakpoint
    // =========================================================================

    bool PapyrusAddAttributeBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex,
        std::int32_t level,
        std::int32_t value)
    {
        if (
            !Settings::GetUseAttributesAtLevelUp() ||
            !IsValidAttributeTableIndex(tableIndex) ||
            !Papyrus::IsValidBreakpointLevel(level) ||
            !IsValidAttributeValue(value))
        {
            return false;
        }

        const auto table =
            static_cast<std::size_t>(
                tableIndex);

        auto breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    table);

        if (
            breakpoints.size() >=
            Settings::MAX_ATTRIBUTE_BREAKPOINTS)
        {
            SKSE::log::warn(
                "AddAttributeBreakpoint rejected: "
                "maximum breakpoint count reached for table {}.",
                tableIndex);

            return false;
        }

        breakpoints.push_back(
            Settings::AttributeBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    value)});

        SortAttributeBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateAttributeBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "AddAttributeBreakpoint rejected "
                "duplicate level {} for table {}.",
                level,
                tableIndex);

            return false;
        }

        const bool result =
            Settings::
                SetAttributeBreakpoints(
                    table,
                    breakpoints);

        SKSE::log::info(
            "Papyrus AddAttributeBreakpoint("
            "{}, {}, {}) -> {}",
            tableIndex,
            level,
            value,
            result);

        return result;
    }

    // =========================================================================
    // Attributes at level up - remove breakpoint
    // =========================================================================

    bool PapyrusRemoveAttributeBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t tableIndex,
        std::int32_t index)
    {
        if (
            !Settings::GetUseAttributesAtLevelUp() ||
            !IsValidAttributeTableIndex(tableIndex) ||
            index <= 0)
        {
            return false;
        }

        const auto table =
            static_cast<std::size_t>(
                tableIndex);

        auto breakpoints =
            Settings::
                GetAttributeBreakpoints(
                    table);

        const auto breakpointIndex =
            static_cast<std::size_t>(
                index);

        if (
            breakpointIndex >=
                breakpoints.size() ||
            breakpoints.size() <= 1)
        {
            return false;
        }

        breakpoints.erase(
            breakpoints.begin() +
            static_cast<std::ptrdiff_t>(
                breakpointIndex));

        const bool result =
            Settings::
                SetAttributeBreakpoints(
                    table,
                    breakpoints);

        SKSE::log::info(
            "Papyrus RemoveAttributeBreakpoint("
            "{}, {}) -> {}",
            tableIndex,
            index,
            result);

        return result;
    }

    bool PapyrusGetIniUseAttributesAtLevelUp(
        RE::StaticFunctionTag *)
    {
        return Settings::
            GetUseAttributesAtLevelUp();
    }

    // =========================================================================
    // Legendary settings
    // =========================================================================

    bool PapyrusGetIniUseLegendarySettings(
        RE::StaticFunctionTag *)
    {
        return Settings::
            GetUseLegendarySettings();
    }

    bool PapyrusGetLegendaryKeepSkillLevel(
        RE::StaticFunctionTag *)
    {
        return Settings::
            GetLegendarySettings()
            .keepSkillLevel;
    }

    bool PapyrusGetLegendaryHideButton(
        RE::StaticFunctionTag *)
    {
        return Settings::
            GetLegendarySettings()
            .hideLegendaryButton;
    }

    std::int32_t PapyrusGetLegendarySkillLevel(
        RE::StaticFunctionTag *)
    {
        return static_cast<std::int32_t>(
            Settings::
                GetLegendarySettings()
                .skillLevelEnable);
    }

    std::int32_t PapyrusGetLegendarySkillLevelAfter(
        RE::StaticFunctionTag *)
    {
        return static_cast<std::int32_t>(
            Settings::
                GetLegendarySettings()
                .skillLevelAfter);
    }

    bool PapyrusSetLegendarySettings(
        RE::StaticFunctionTag *,
        bool keepSkillLevel,
        bool hideButton,
        std::int32_t skillLevel,
        std::int32_t skillLevelAfter)
    {
        if (
            !Settings::GetUseLegendarySettings() ||
            skillLevel <
                static_cast<std::int32_t>(
                    Settings::MIN_LEGENDARY_SKILL_LEVEL) ||
            skillLevel >
                static_cast<std::int32_t>(
                    Settings::MAX_LEGENDARY_SKILL_LEVEL) ||
            skillLevelAfter <
                static_cast<std::int32_t>(
                    Settings::MIN_AFTER_LEGENDARY_LEVEL) ||
            skillLevelAfter >
                static_cast<std::int32_t>(
                    Settings::MAX_AFTER_LEGENDARY_LEVEL))
        {
            return false;
        }

        const Settings::LegendarySettings legendary{
            keepSkillLevel,
            hideButton,
            static_cast<std::uint32_t>(
                skillLevel),
            static_cast<std::uint32_t>(
                skillLevelAfter)};

        const bool result =
            Settings::
                SetLegendarySettings(
                    legendary);

        SKSE::log::info(
            "Papyrus SetLegendarySettings({}, {}, {}, {}) -> {}",
            keepSkillLevel,
            hideButton,
            skillLevel,
            skillLevelAfter,
            result);

        return result;
    }

    // =========================================================================
    // General
    // =========================================================================

    bool PapyrusClearOverrides(
        RE::StaticFunctionTag *)
    {
        const bool result =
            Settings::Reset();

        SKSE::log::info(
            "Papyrus ClearOverrides -> {}",
            result);

        return result;
    }

    bool PapyrusGetEnabled(
        RE::StaticFunctionTag *)
    {
        return Settings::IsEnabled();
    }

    bool PapyrusSetEnabled(
        RE::StaticFunctionTag *,
        bool enabled)
    {
        const bool result =
            Settings::SetEnabled(
                enabled);

        SKSE::log::info(
            "Papyrus SetEnabled({}) -> {}",
            enabled,
            result);

        return result;
    }
}

// =============================================================================
// Papyrus registration
// =============================================================================

bool RegisterPapyrus(
    RE::BSScript::IVirtualMachine *vm)
{
    SKSE::log::info(
        "Registering Papyrus functions...");

    // -------------------------------------------------------------------------
    // Caps
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "SetSkillCap",
        PAPYRUS_CLASS,
        PapyrusSetSkillCap);

    vm->RegisterFunction(
        "SetFormulaCap",
        PAPYRUS_CLASS,
        PapyrusSetFormulaCap);

    vm->RegisterFunction(
        "GetSkillCap",
        PAPYRUS_CLASS,
        PapyrusGetSkillCap);

    vm->RegisterFunction(
        "GetFormulaCap",
        PAPYRUS_CLASS,
        PapyrusGetFormulaCap);

    // -------------------------------------------------------------------------
    // Enchanting
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "SetEnchantMagnitudeCap",
        PAPYRUS_CLASS,
        PapyrusSetEnchantMagnitudeCap);

    vm->RegisterFunction(
        "SetEnchantChargeCap",
        PAPYRUS_CLASS,
        PapyrusSetEnchantChargeCap);

    vm->RegisterFunction(
        "SetEnchantUseLinearCharge",
        PAPYRUS_CLASS,
        PapyrusSetEnchantUseLinearCharge);

    vm->RegisterFunction(
        "GetEnchantMagnitudeCap",
        PAPYRUS_CLASS,
        PapyrusGetEnchantMagnitudeCap);

    vm->RegisterFunction(
        "GetEnchantChargeCap",
        PAPYRUS_CLASS,
        PapyrusGetEnchantChargeCap);

    vm->RegisterFunction(
        "GetEnchantUseLinearCharge",
        PAPYRUS_CLASS,
        PapyrusGetEnchantUseLinearCharge);

    // -------------------------------------------------------------------------
    // Skill XP
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "GetSkillExpBaseMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpBaseMultiplier);

    vm->RegisterFunction(
        "GetSkillExpOffsetMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpOffsetMultiplier);

    vm->RegisterFunction(
        "SetSkillExpBaseMultipliers",
        PAPYRUS_CLASS,
        PapyrusSetSkillExpBaseMultipliers);

    vm->RegisterFunction(
        "GetSkillExpBreakpointCount",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpBreakpointCount);

    vm->RegisterFunction(
        "GetSkillExpBreakpointLevel",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpBreakpointLevel);

    vm->RegisterFunction(
        "GetSkillExpBreakpointBaseMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpBreakpointBaseMultiplier);

    vm->RegisterFunction(
        "GetSkillExpBreakpointOffsetMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetSkillExpBreakpointOffsetMultiplier);

    vm->RegisterFunction(
        "SetSkillExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusSetSkillExpBreakpoint);

    vm->RegisterFunction(
        "AddSkillExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusAddSkillExpBreakpoint);

    vm->RegisterFunction(
        "RemoveSkillExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusRemoveSkillExpBreakpoint);

    // -------------------------------------------------------------------------
    // Player Level XP
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "GetPlayerLevelExpMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetPlayerLevelExpMultiplier);

    vm->RegisterFunction(
        "SetPlayerLevelExpMultiplier",
        PAPYRUS_CLASS,
        PapyrusSetPlayerLevelExpMultiplier);

    vm->RegisterFunction(
        "GetPlayerLevelExpBreakpointCount",
        PAPYRUS_CLASS,
        PapyrusGetPlayerLevelExpBreakpointCount);

    vm->RegisterFunction(
        "GetPlayerLevelExpBreakpointLevel",
        PAPYRUS_CLASS,
        PapyrusGetPlayerLevelExpBreakpointLevel);

    vm->RegisterFunction(
        "GetPlayerLevelExpBreakpointMultiplier",
        PAPYRUS_CLASS,
        PapyrusGetPlayerLevelExpBreakpointMultiplier);

    vm->RegisterFunction(
        "SetPlayerLevelExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusSetPlayerLevelExpBreakpoint);

    vm->RegisterFunction(
        "AddPlayerLevelExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusAddPlayerLevelExpBreakpoint);

    vm->RegisterFunction(
        "RemovePlayerLevelExpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusRemovePlayerLevelExpBreakpoint);

    // -------------------------------------------------------------------------
    // Perks at level up
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "GetPerksAtLevelUpBreakpointCount",
        PAPYRUS_CLASS,
        PapyrusGetPerksAtLevelUpBreakpointCount);

    vm->RegisterFunction(
        "GetPerksAtLevelUpBreakpointLevel",
        PAPYRUS_CLASS,
        PapyrusGetPerksAtLevelUpBreakpointLevel);

    vm->RegisterFunction(
        "GetPerksAtLevelUpBreakpointValue",
        PAPYRUS_CLASS,
        PapyrusGetPerksAtLevelUpBreakpointValue);

    vm->RegisterFunction(
        "SetPerksAtLevelUpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusSetPerksAtLevelUpBreakpoint);

    vm->RegisterFunction(
        "AddPerksAtLevelUpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusAddPerksAtLevelUpBreakpoint);

    vm->RegisterFunction(
        "RemovePerksAtLevelUpBreakpoint",
        PAPYRUS_CLASS,
        PapyrusRemovePerksAtLevelUpBreakpoint);

    // -------------------------------------------------------------------------
    // Attributes at level up
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "GetAttributeBreakpointCount",
        PAPYRUS_CLASS,
        PapyrusGetAttributeBreakpointCount);

    vm->RegisterFunction(
        "GetAttributeBreakpointLevel",
        PAPYRUS_CLASS,
        PapyrusGetAttributeBreakpointLevel);

    vm->RegisterFunction(
        "GetAttributeBreakpointValue",
        PAPYRUS_CLASS,
        PapyrusGetAttributeBreakpointValue);

    vm->RegisterFunction(
        "SetAttributeBreakpoint",
        PAPYRUS_CLASS,
        PapyrusSetAttributeBreakpoint);

    vm->RegisterFunction(
        "AddAttributeBreakpoint",
        PAPYRUS_CLASS,
        PapyrusAddAttributeBreakpoint);

    vm->RegisterFunction(
        "RemoveAttributeBreakpoint",
        PAPYRUS_CLASS,
        PapyrusRemoveAttributeBreakpoint);

    vm->RegisterFunction(
        "GetIniUseAttributesAtLevelUp",
        PAPYRUS_CLASS,
        PapyrusGetIniUseAttributesAtLevelUp);

    // -------------------------------------------------------------------------
    // Legendary settings
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "GetIniUseLegendarySettings",
        PAPYRUS_CLASS,
        PapyrusGetIniUseLegendarySettings);

    vm->RegisterFunction(
        "GetLegendaryKeepSkillLevel",
        PAPYRUS_CLASS,
        PapyrusGetLegendaryKeepSkillLevel);

    vm->RegisterFunction(
        "GetLegendaryHideButton",
        PAPYRUS_CLASS,
        PapyrusGetLegendaryHideButton);

    vm->RegisterFunction(
        "GetLegendarySkillLevel",
        PAPYRUS_CLASS,
        PapyrusGetLegendarySkillLevel);

    vm->RegisterFunction(
        "GetLegendarySkillLevelAfter",
        PAPYRUS_CLASS,
        PapyrusGetLegendarySkillLevelAfter);

    vm->RegisterFunction(
        "SetLegendarySettings",
        PAPYRUS_CLASS,
        PapyrusSetLegendarySettings);

    // -------------------------------------------------------------------------
    // General
    // -------------------------------------------------------------------------

    vm->RegisterFunction(
        "ClearOverrides",
        PAPYRUS_CLASS,
        PapyrusClearOverrides);

    vm->RegisterFunction(
        "GetEnabled",
        PAPYRUS_CLASS,
        PapyrusGetEnabled);

    vm->RegisterFunction(
        "SetEnabled",
        PAPYRUS_CLASS,
        PapyrusSetEnabled);

    SKSE::log::info(
        "Papyrus functions registered.");

    return true;
}

// =============================================================================
// Plugin load
// =============================================================================

SKSEPluginLoad(
    const SKSE::LoadInterface *skse)
{
    SetupLog();

    SKSE::Init(skse);

    Serialization::Register();

    SKSE::log::info(
        "UncapperMCM loaded successfully.");

    if (!UncapperAPI::IsAvailable())
    {
        SKSE::log::error(
            "SkyrimUncapper.dll was not found.");

        return false;
    }

    SKSE::log::info(
        "SkyrimUncapper.dll detected.");

    if (!Settings::LoadFromIni())
    {
        SKSE::log::error(
            "Unable to initialize settings "
            "from SkyrimUncapper.ini.");

        return false;
    }

    SKSE::log::info(
        "Settings initialized "
        "from SkyrimUncapper.ini.");

    // -------------------------------------------------------------------------
    // Startup diagnostics
    // -------------------------------------------------------------------------

    const auto oneHandedSkillCap =
        UncapperAPI::GetIniSkillCap(0);

    const auto oneHandedFormulaCap =
        UncapperAPI::GetIniFormulaCap(0);

    const auto enchantMagnitudeCap =
        UncapperAPI::
            GetIniEnchantMagnitudeCap();

    const auto enchantChargeCap =
        UncapperAPI::
            GetIniEnchantChargeCap();

    const auto enchantLinearCharge =
        UncapperAPI::
            GetIniEnchantUseLinearCharge();

    const auto oneHandedSkillExpBase =
        UncapperAPI::
            GetIniSkillExpBaseMultiplier(0);

    const auto oneHandedSkillExpOffset =
        UncapperAPI::
            GetIniSkillExpOffsetMultiplier(0);

    const auto oneHandedSkillLevelBreakpointCount =
        UncapperAPI::
            GetIniSkillExpSkillLevelBreakpointCount(
                0);

    const auto oneHandedCharacterLevelBreakpointCount =
        UncapperAPI::
            GetIniSkillExpCharacterLevelBreakpointCount(
                0);

    const auto oneHandedPlayerLevelExp =
        UncapperAPI::
            GetIniLevelExpMultiplier(0);

    const auto oneHandedPlayerLevelExpSkillBreakpointCount =
        UncapperAPI::
            GetIniLevelExpSkillLevelBreakpointCount(
                0);

    const auto oneHandedPlayerLevelExpCharacterBreakpointCount =
        UncapperAPI::
            GetIniLevelExpCharacterLevelBreakpointCount(
                0);

    SKSE::log::info(
        "INI OneHanded SkillCap = {}",
        oneHandedSkillCap);

    SKSE::log::info(
        "INI OneHanded FormulaCap = {}",
        oneHandedFormulaCap);

    SKSE::log::info(
        "INI Enchanting MagnitudeCap = {}",
        enchantMagnitudeCap);

    SKSE::log::info(
        "INI Enchanting ChargeCap = {}",
        enchantChargeCap);

    SKSE::log::info(
        "INI Enchanting UseLinearCharge = {}",
        enchantLinearCharge);

    SKSE::log::info(
        "INI OneHanded SkillExp Base/Offset = {}/{}",
        oneHandedSkillExpBase,
        oneHandedSkillExpOffset);

    SKSE::log::info(
        "INI OneHanded SkillExp breakpoint counts: "
        "SkillLevel={}, CharacterLevel={}",
        oneHandedSkillLevelBreakpointCount,
        oneHandedCharacterLevelBreakpointCount);

    SKSE::log::info(
        "INI OneHanded Player Level XP multiplier = {}",
        oneHandedPlayerLevelExp);

    SKSE::log::info(
        "INI OneHanded Player Level XP breakpoint counts: "
        "SkillLevel={}, CharacterLevel={}",
        oneHandedPlayerLevelExpSkillBreakpointCount,
        oneHandedPlayerLevelExpCharacterBreakpointCount);

    // -------------------------------------------------------------------------
    // Papyrus
    // -------------------------------------------------------------------------

    const auto papyrus =
        SKSE::GetPapyrusInterface();

    if (!papyrus)
    {
        SKSE::log::error(
            "Papyrus interface was not found.");

        return false;
    }

    if (!papyrus->Register(
            RegisterPapyrus))
    {
        SKSE::log::error(
            "Unable to register "
            "Papyrus functions.");

        return false;
    }

    SKSE::log::info(
        "UncapperMCM initialization complete.");

    return true;
}
