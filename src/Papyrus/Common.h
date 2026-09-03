#pragma once

#include <cstdint>
#include <vector>

namespace Settings
{
    struct MultiplierBreakpoint;
}

namespace Papyrus
{
    bool IsValidSkillSlot(
        std::int32_t skillSlot);

    bool IsValidMultiplier(
        std::int32_t value);

    bool IsValidBreakpointLevel(
        std::int32_t level);

    void SortMultiplierBreakpoints(
        std::vector<Settings::MultiplierBreakpoint> &breakpoints);

    bool ContainsDuplicateMultiplierBreakpointLevels(
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints);
}
