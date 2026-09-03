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
}
