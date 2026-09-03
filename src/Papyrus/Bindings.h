#pragma once

#include <cstdint>

namespace RE
{
    class StaticFunctionTag;
}

bool PapyrusSetSkillCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t value);

bool PapyrusSetFormulaCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t value);

std::int32_t PapyrusGetSkillCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot);

std::int32_t PapyrusGetFormulaCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot);
