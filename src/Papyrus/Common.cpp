#include "../PCH.h"

#include "Common.h"
#include "Settings.h"

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
}
