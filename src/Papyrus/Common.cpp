#include "../PCH.h"

#include "Common.h"
#include "Settings.h"

#include <algorithm>

namespace Papyrus
{
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
}
