#include "../PCH.h"

#include "Bindings.h"
#include "Common.h"
#include "Settings.h"

#include <algorithm>

namespace
{
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
}

std::int32_t PapyrusGetSkillExpBaseMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
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
    if (!Papyrus::IsValidSkillSlot(skillSlot))
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
    if (!Papyrus::IsValidSkillSlot(skillSlot))
    {
        SKSE::log::warn(
            "SetSkillExpBaseMultipliers: "
            "invalid skill slot {}",
            skillSlot);

        return false;
    }

    if (
        !Papyrus::IsValidMultiplier(
            baseHundredths) ||
        !Papyrus::IsValidMultiplier(
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

std::int32_t PapyrusGetSkillExpBreakpointCount(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
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
        !Papyrus::IsValidSkillSlot(skillSlot) ||
        index < 0 ||
        !Papyrus::IsValidBreakpointLevel(level) ||
        !Papyrus::IsValidMultiplier(
            baseHundredths) ||
        !Papyrus::IsValidMultiplier(
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

bool PapyrusAddSkillExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t level,
    std::int32_t baseHundredths,
    std::int32_t offsetHundredths)
{
    if (
        !Papyrus::IsValidSkillSlot(skillSlot) ||
        !Papyrus::IsValidBreakpointLevel(level) ||
        !Papyrus::IsValidMultiplier(
            baseHundredths) ||
        !Papyrus::IsValidMultiplier(
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

bool PapyrusRemoveSkillExpBreakpoint(
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

