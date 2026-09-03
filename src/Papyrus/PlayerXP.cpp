#include "../PCH.h"

#include "Bindings.h"
#include "Common.h"
#include "Settings.h"

namespace
{
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
}

std::int32_t PapyrusGetPlayerLevelExpMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
        !Papyrus::IsValidMultiplier(
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

std::int32_t PapyrusGetPlayerLevelExpBreakpointCount(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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

bool PapyrusSetPlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index,
    std::int32_t level,
    std::int32_t multiplierHundredths)
{
    if (
        !Papyrus::IsValidSkillSlot(skillSlot) ||
        index < 0 ||
        !Papyrus::IsValidBreakpointLevel(level) ||
        !Papyrus::IsValidMultiplier(
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

    Papyrus::SortMultiplierBreakpoints(
        breakpoints);

    if (
        Papyrus::ContainsDuplicateMultiplierBreakpointLevels(
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

bool PapyrusAddPlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t level,
    std::int32_t multiplierHundredths)
{
    if (
        !Papyrus::IsValidSkillSlot(skillSlot) ||
        !Papyrus::IsValidBreakpointLevel(level) ||
        !Papyrus::IsValidMultiplier(
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

    Papyrus::SortMultiplierBreakpoints(
        breakpoints);

    if (
        Papyrus::ContainsDuplicateMultiplierBreakpointLevels(
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

bool PapyrusRemovePlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index)
{
    if (
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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

