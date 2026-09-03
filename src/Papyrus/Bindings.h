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

bool PapyrusSetEnchantMagnitudeCap(
    RE::StaticFunctionTag *,
    std::int32_t value);

bool PapyrusSetEnchantChargeCap(
    RE::StaticFunctionTag *,
    std::int32_t value);

void PapyrusSetEnchantUseLinearCharge(
    RE::StaticFunctionTag *,
    bool enabled);

std::int32_t PapyrusGetEnchantMagnitudeCap(
    RE::StaticFunctionTag *);

std::int32_t PapyrusGetEnchantChargeCap(
    RE::StaticFunctionTag *);

bool PapyrusGetEnchantUseLinearCharge(
    RE::StaticFunctionTag *);

std::int32_t PapyrusGetSkillExpBaseMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot);

std::int32_t PapyrusGetSkillExpOffsetMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot);

bool PapyrusSetSkillExpBaseMultipliers(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t baseHundredths,
    std::int32_t offsetHundredths);

std::int32_t PapyrusGetSkillExpBreakpointCount(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel);

std::int32_t PapyrusGetSkillExpBreakpointLevel(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

std::int32_t PapyrusGetSkillExpBreakpointBaseMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

std::int32_t PapyrusGetSkillExpBreakpointOffsetMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

bool PapyrusSetSkillExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index,
    std::int32_t level,
    std::int32_t baseHundredths,
    std::int32_t offsetHundredths);

bool PapyrusAddSkillExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t level,
    std::int32_t baseHundredths,
    std::int32_t offsetHundredths);

bool PapyrusRemoveSkillExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

std::int32_t PapyrusGetPlayerLevelExpMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot);

bool PapyrusSetPlayerLevelExpMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t multiplierHundredths);

std::int32_t PapyrusGetPlayerLevelExpBreakpointCount(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel);

std::int32_t PapyrusGetPlayerLevelExpBreakpointLevel(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

std::int32_t PapyrusGetPlayerLevelExpBreakpointMultiplier(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

bool PapyrusSetPlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index,
    std::int32_t level,
    std::int32_t multiplierHundredths);

bool PapyrusAddPlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t level,
    std::int32_t multiplierHundredths);

bool PapyrusRemovePlayerLevelExpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    bool characterLevel,
    std::int32_t index);

std::int32_t PapyrusGetPerksAtLevelUpBreakpointCount(
    RE::StaticFunctionTag *);

std::int32_t PapyrusGetPerksAtLevelUpBreakpointLevel(
    RE::StaticFunctionTag *,
    std::int32_t index);

std::int32_t PapyrusGetPerksAtLevelUpBreakpointValue(
    RE::StaticFunctionTag *,
    std::int32_t index);

bool PapyrusSetPerksAtLevelUpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t index,
    std::int32_t level,
    std::int32_t valueHundredths);

bool PapyrusAddPerksAtLevelUpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t level,
    std::int32_t valueHundredths);

bool PapyrusRemovePerksAtLevelUpBreakpoint(
    RE::StaticFunctionTag *,
    std::int32_t index);
