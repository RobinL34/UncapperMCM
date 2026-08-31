#include "PCH.h"
#include "UncapperAPI.h"
#include "Settings.h"
#include "Serialization.h"

#include <algorithm>
#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    constexpr auto PAPYRUS_CLASS =
        "UncapperMCM";

    // =========================================================================
    // Helpers
    // =========================================================================

    bool IsValidSkillSlot(
        std::int32_t skillSlot)
    {
        return skillSlot >= 0 &&
               skillSlot <
                   static_cast<std::int32_t>(
                       Settings::SKILL_COUNT);
    }

    bool IsValidMultiplier(
        std::int32_t value)
    {
        return value >= static_cast<std::int32_t>(
                            Settings::MIN_MULTIPLIER_HUNDREDTHS) &&
               value <= static_cast<std::int32_t>(
                            Settings::MAX_MULTIPLIER_HUNDREDTHS);
    }

    bool IsValidBreakpointLevel(
        std::int32_t level)
    {
        return level >= static_cast<std::int32_t>(
                            Settings::MIN_BREAKPOINT_LEVEL) &&
               level <= static_cast<std::int32_t>(
                            Settings::MAX_BREAKPOINT_LEVEL);
    }

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

    const std::vector<Settings::SkillExpBreakpoint> &
    GetSkillExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel)
    {
        if (characterLevel)
        {
            return Settings::
                GetSkillExpCharacterLevelBreakpoints(
                    skillSlot);
        }

        return Settings::
            GetSkillExpSkillLevelBreakpoints(
                skillSlot);
    }

    bool SetSkillExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel,
        const std::vector<Settings::SkillExpBreakpoint> &breakpoints)
    {
        if (characterLevel)
        {
            return Settings::
                SetSkillExpCharacterLevelBreakpoints(
                    skillSlot,
                    breakpoints);
        }

        return Settings::
            SetSkillExpSkillLevelBreakpoints(
                skillSlot,
                breakpoints);
    }

    void SortSkillExpBreakpoints(
        std::vector<Settings::SkillExpBreakpoint> &breakpoints)
    {
        std::sort(
            breakpoints.begin(),
            breakpoints.end(),
            [](
                const Settings::SkillExpBreakpoint &a,
                const Settings::SkillExpBreakpoint &b)
            {
                return a.level < b.level;
            });
    }

    bool ContainsDuplicateBreakpointLevels(
        const std::vector<Settings::SkillExpBreakpoint> &breakpoints)
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

    const std::vector<Settings::MultiplierBreakpoint> &
    GetPlayerLevelExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel)
    {
        if (characterLevel)
        {
            return Settings::
                GetPlayerLevelExpCharacterLevelBreakpoints(
                    skillSlot);
        }

        return Settings::
            GetPlayerLevelExpSkillLevelBreakpoints(
                skillSlot);
    }

    bool SetPlayerLevelExpBreakpointTable(
        std::size_t skillSlot,
        bool characterLevel,
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (characterLevel)
        {
            return Settings::
                SetPlayerLevelExpCharacterLevelBreakpoints(
                    skillSlot,
                    breakpoints);
        }

        return Settings::
            SetPlayerLevelExpSkillLevelBreakpoints(
                skillSlot,
                breakpoints);
    }

    void SortMultiplierBreakpoints(
        std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        std::sort(
            breakpoints.begin(),
            breakpoints.end(),
            [](
                const Settings::MultiplierBreakpoint &a,
                const Settings::MultiplierBreakpoint &b)
            {
                return a.level < b.level;
            });
    }

    bool ContainsDuplicateMultiplierBreakpointLevels(
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
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
    // Skill Caps
    // =========================================================================

    bool PapyrusSetSkillCap(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        std::int32_t value)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            SKSE::log::warn(
                "SetSkillCap: invalid skill slot {}",
                skillSlot);

            return false;
        }

        if (value < 1)
        {
            SKSE::log::warn(
                "SetSkillCap: invalid value {}",
                value);

            return false;
        }

        const bool result =
            Settings::SetSkillCap(
                static_cast<std::size_t>(
                    skillSlot),
                static_cast<std::uint32_t>(
                    value));

        SKSE::log::info(
            "Papyrus SetSkillCap({}, {}) -> {}",
            skillSlot,
            value,
            result);

        return result;
    }

    bool PapyrusSetFormulaCap(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        std::int32_t value)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            SKSE::log::warn(
                "SetFormulaCap: invalid skill slot {}",
                skillSlot);

            return false;
        }

        if (value < 1)
        {
            SKSE::log::warn(
                "SetFormulaCap: invalid value {}",
                value);

            return false;
        }

        const bool result =
            Settings::SetFormulaCap(
                static_cast<std::size_t>(
                    skillSlot),
                static_cast<std::uint32_t>(
                    value));

        SKSE::log::info(
            "Papyrus SetFormulaCap({}, {}) -> {}",
            skillSlot,
            value,
            result);

        return result;
    }

    std::int32_t PapyrusGetSkillCap(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            Settings::GetSkillCap(
                static_cast<std::size_t>(
                    skillSlot)));
    }

    std::int32_t PapyrusGetFormulaCap(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            Settings::GetFormulaCap(
                static_cast<std::size_t>(
                    skillSlot)));
    }

    // =========================================================================
    // Enchanting
    // =========================================================================

    bool PapyrusSetEnchantMagnitudeCap(
        RE::StaticFunctionTag *,
        std::int32_t value)
    {
        if (value < 1)
        {
            SKSE::log::warn(
                "SetEnchantMagnitudeCap: invalid value {}",
                value);

            return false;
        }

        const bool result =
            Settings::SetEnchantMagnitudeCap(
                static_cast<std::uint32_t>(
                    value));

        SKSE::log::info(
            "Papyrus SetEnchantMagnitudeCap({}) -> {}",
            value,
            result);

        return result;
    }

    bool PapyrusSetEnchantChargeCap(
        RE::StaticFunctionTag *,
        std::int32_t value)
    {
        if (value < 1)
        {
            SKSE::log::warn(
                "SetEnchantChargeCap: invalid value {}",
                value);

            return false;
        }

        const bool result =
            Settings::SetEnchantChargeCap(
                static_cast<std::uint32_t>(
                    value));

        SKSE::log::info(
            "Papyrus SetEnchantChargeCap({}) -> {}",
            value,
            result);

        return result;
    }

    void PapyrusSetEnchantUseLinearCharge(
        RE::StaticFunctionTag *,
        bool enabled)
    {
        const bool result =
            Settings::SetEnchantUseLinearCharge(
                enabled);

        SKSE::log::info(
            "Papyrus SetEnchantUseLinearCharge({}) -> {}",
            enabled,
            result);
    }

    std::int32_t PapyrusGetEnchantMagnitudeCap(
        RE::StaticFunctionTag *)
    {
        return static_cast<std::int32_t>(
            Settings::GetEnchantMagnitudeCap());
    }

    std::int32_t PapyrusGetEnchantChargeCap(
        RE::StaticFunctionTag *)
    {
        return static_cast<std::int32_t>(
            Settings::GetEnchantChargeCap());
    }

    bool PapyrusGetEnchantUseLinearCharge(
        RE::StaticFunctionTag *)
    {
        return Settings::
            GetEnchantUseLinearCharge();
    }

    // =========================================================================
    // Skill XP - base / offset multipliers
    // =========================================================================

    std::int32_t PapyrusGetSkillExpBaseMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            Settings::
                GetSkillExpBaseMultiplier(
                    static_cast<std::size_t>(
                        skillSlot)));
    }

    std::int32_t PapyrusGetSkillExpOffsetMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            Settings::
                GetSkillExpOffsetMultiplier(
                    static_cast<std::size_t>(
                        skillSlot)));
    }

    bool PapyrusSetSkillExpBaseMultipliers(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        std::int32_t baseHundredths,
        std::int32_t offsetHundredths)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            SKSE::log::warn(
                "SetSkillExpBaseMultipliers: "
                "invalid skill slot {}",
                skillSlot);

            return false;
        }

        if (
            !IsValidMultiplier(
                baseHundredths) ||
            !IsValidMultiplier(
                offsetHundredths))
        {
            SKSE::log::warn(
                "SetSkillExpBaseMultipliers: "
                "invalid multipliers {}/{}",
                baseHundredths,
                offsetHundredths);

            return false;
        }

        const bool result =
            Settings::
                SetSkillExpBaseMultipliers(
                    static_cast<std::size_t>(
                        skillSlot),
                    static_cast<std::uint32_t>(
                        baseHundredths),
                    static_cast<std::uint32_t>(
                        offsetHundredths));

        SKSE::log::info(
            "Papyrus SetSkillExpBaseMultipliers({}, {}, {}) -> {}",
            skillSlot,
            baseHundredths,
            offsetHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Skill XP - breakpoint getters
    //
    // characterLevel = false:
    //      breakpoints based on the skill's base level
    //
    // characterLevel = true:
    //      breakpoints based on the player's character level
    // =========================================================================

    std::int32_t PapyrusGetSkillExpBreakpointCount(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        const auto &breakpoints =
            GetSkillExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

        return static_cast<std::int32_t>(
            breakpoints.size());
    }

    std::int32_t PapyrusGetSkillExpBreakpointLevel(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            GetSkillExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

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

    std::int32_t PapyrusGetSkillExpBreakpointBaseMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            GetSkillExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

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
                .baseHundredths);
    }

    std::int32_t PapyrusGetSkillExpBreakpointOffsetMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            GetSkillExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

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
                .offsetHundredths);
    }

    // =========================================================================
    // Skill XP - modify existing breakpoint
    // =========================================================================

    bool PapyrusSetSkillExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index,
        std::int32_t level,
        std::int32_t baseHundredths,
        std::int32_t offsetHundredths)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0 ||
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
                baseHundredths) ||
            !IsValidMultiplier(
                offsetHundredths))
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetSkillExpBreakpointTable(
                slot,
                characterLevel);

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
            breakpoints[breakpointIndex].level == 0 &&
            level != 0)
        {
            SKSE::log::warn(
                "SetSkillExpBreakpoint rejected: "
                "level 0 breakpoint cannot be moved.");

            return false;
        }

        breakpoints[breakpointIndex] =
            Settings::SkillExpBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    baseHundredths),
                static_cast<std::uint32_t>(
                    offsetHundredths)};

        SortSkillExpBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "SetSkillExpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            SetSkillExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus SetSkillExpBreakpoint("
            "{}, {}, {}, {}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            index,
            level,
            baseHundredths,
            offsetHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Skill XP - add breakpoint
    // =========================================================================

    bool PapyrusAddSkillExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t level,
        std::int32_t baseHundredths,
        std::int32_t offsetHundredths)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
                baseHundredths) ||
            !IsValidMultiplier(
                offsetHundredths))
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetSkillExpBreakpointTable(
                slot,
                characterLevel);

        if (
            breakpoints.size() >=
            Settings::
                MAX_SKILL_EXP_BREAKPOINTS)
        {
            SKSE::log::warn(
                "AddSkillExpBreakpoint rejected: "
                "maximum breakpoint count reached.");

            return false;
        }

        breakpoints.push_back(
            Settings::SkillExpBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    baseHundredths),
                static_cast<std::uint32_t>(
                    offsetHundredths)});

        SortSkillExpBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "AddSkillExpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            SetSkillExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus AddSkillExpBreakpoint("
            "{}, {}, {}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            level,
            baseHundredths,
            offsetHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Skill XP - remove breakpoint
    // =========================================================================

    bool PapyrusRemoveSkillExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetSkillExpBreakpointTable(
                slot,
                characterLevel);

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
                "RemoveSkillExpBreakpoint rejected: "
                "level 0 breakpoint cannot be removed.");

            return false;
        }

        // Runtime tables must always contain at least
        // one breakpoint.
        if (
            breakpoints.size() <= 1)
        {
            SKSE::log::warn(
                "RemoveSkillExpBreakpoint rejected: "
                "the last breakpoint cannot be removed.");

            return false;
        }

        breakpoints.erase(
            breakpoints.begin() +
            static_cast<std::ptrdiff_t>(
                breakpointIndex));

        const bool result =
            SetSkillExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus RemoveSkillExpBreakpoint("
            "{}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            index,
            result);

        return result;
    }

    // =========================================================================
    // Player Level XP - base multiplier
    // =========================================================================

    std::int32_t PapyrusGetPlayerLevelExpMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        return static_cast<std::int32_t>(
            Settings::
                GetPlayerLevelExpMultiplier(
                    static_cast<std::size_t>(
                        skillSlot)));
    }

    bool PapyrusSetPlayerLevelExpMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        std::int32_t multiplierHundredths)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            !IsValidMultiplier(
                multiplierHundredths))
        {
            return false;
        }

        const bool result =
            Settings::
                SetPlayerLevelExpMultiplier(
                    static_cast<std::size_t>(
                        skillSlot),
                    static_cast<std::uint32_t>(
                        multiplierHundredths));

        SKSE::log::info(
            "Papyrus SetPlayerLevelExpMultiplier({}, {}) -> {}",
            skillSlot,
            multiplierHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Player Level XP - breakpoint getters
    // =========================================================================

    std::int32_t PapyrusGetPlayerLevelExpBreakpointCount(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel)
    {
        if (!IsValidSkillSlot(skillSlot))
        {
            return -1;
        }

        const auto &breakpoints =
            GetPlayerLevelExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

        return static_cast<std::int32_t>(
            breakpoints.size());
    }

    std::int32_t PapyrusGetPlayerLevelExpBreakpointLevel(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            GetPlayerLevelExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

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

    std::int32_t PapyrusGetPlayerLevelExpBreakpointMultiplier(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return -1;
        }

        const auto &breakpoints =
            GetPlayerLevelExpBreakpointTable(
                static_cast<std::size_t>(
                    skillSlot),
                characterLevel);

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
    // Player Level XP - modify breakpoint
    // =========================================================================

    bool PapyrusSetPlayerLevelExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index,
        std::int32_t level,
        std::int32_t multiplierHundredths)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0 ||
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
                multiplierHundredths))
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel);

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
            breakpoints[breakpointIndex].level == 0 &&
            level != 0)
        {
            SKSE::log::warn(
                "SetPlayerLevelExpBreakpoint rejected: "
                "level 0 breakpoint cannot be moved.");

            return false;
        }

        breakpoints[breakpointIndex] =
            Settings::MultiplierBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    multiplierHundredths)};

        SortMultiplierBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateMultiplierBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "SetPlayerLevelExpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            SetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus SetPlayerLevelExpBreakpoint("
            "{}, {}, {}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            index,
            level,
            multiplierHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Player Level XP - add breakpoint
    // =========================================================================

    bool PapyrusAddPlayerLevelExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t level,
        std::int32_t multiplierHundredths)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
                multiplierHundredths))
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel);

        if (
            breakpoints.size() >=
            Settings::
                MAX_LEVEL_EXP_BREAKPOINTS)
        {
            SKSE::log::warn(
                "AddPlayerLevelExpBreakpoint rejected: "
                "maximum breakpoint count reached.");

            return false;
        }

        breakpoints.push_back(
            Settings::MultiplierBreakpoint{
                static_cast<std::uint32_t>(
                    level),
                static_cast<std::uint32_t>(
                    multiplierHundredths)});

        SortMultiplierBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateMultiplierBreakpointLevels(
                breakpoints))
        {
            SKSE::log::warn(
                "AddPlayerLevelExpBreakpoint rejected "
                "duplicate level {}.",
                level);

            return false;
        }

        const bool result =
            SetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus AddPlayerLevelExpBreakpoint("
            "{}, {}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            level,
            multiplierHundredths,
            result);

        return result;
    }

    // =========================================================================
    // Player Level XP - remove breakpoint
    // =========================================================================

    bool PapyrusRemovePlayerLevelExpBreakpoint(
        RE::StaticFunctionTag *,
        std::int32_t skillSlot,
        bool characterLevel,
        std::int32_t index)
    {
        if (
            !IsValidSkillSlot(skillSlot) ||
            index < 0)
        {
            return false;
        }

        const auto slot =
            static_cast<std::size_t>(
                skillSlot);

        auto breakpoints =
            GetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel);

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
                "RemovePlayerLevelExpBreakpoint rejected: "
                "level 0 breakpoint cannot be removed.");

            return false;
        }

        if (
            breakpoints.size() <= 1)
        {
            return false;
        }

        breakpoints.erase(
            breakpoints.begin() +
            static_cast<std::ptrdiff_t>(
                breakpointIndex));

        const bool result =
            SetPlayerLevelExpBreakpointTable(
                slot,
                characterLevel,
                breakpoints);

        SKSE::log::info(
            "Papyrus RemovePlayerLevelExpBreakpoint("
            "{}, {}, {}) -> {}",
            skillSlot,
            characterLevel,
            index,
            result);

        return result;
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
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
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

        SortMultiplierBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateMultiplierBreakpointLevels(
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
            !IsValidBreakpointLevel(level) ||
            !IsValidMultiplier(
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

        SortMultiplierBreakpoints(
            breakpoints);

        if (
            ContainsDuplicateMultiplierBreakpointLevels(
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
            !IsValidBreakpointLevel(level) ||
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
            !IsValidBreakpointLevel(level) ||
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
    // General
    // =========================================================================

    bool PapyrusClearOverrides(
        RE::StaticFunctionTag *)
    {
        Settings::Reset();

        const bool result = true;

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

    void PapyrusSetEnabled(
        RE::StaticFunctionTag *,
        bool enabled)
    {
        Settings::SetEnabled(
            enabled);

        SKSE::log::info(
            "Papyrus SetEnabled({})",
            enabled);
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
