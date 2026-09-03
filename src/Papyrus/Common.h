#pragma once

#include <cstdint>

namespace Papyrus
{
    bool IsValidSkillSlot(
        std::int32_t skillSlot);

    bool IsValidMultiplier(
        std::int32_t value);

    bool IsValidBreakpointLevel(
        std::int32_t level);
}
