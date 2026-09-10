#include "../PCH.h"

#include "Bindings.h"
#include "Common.h"
#include "Settings.h"

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
