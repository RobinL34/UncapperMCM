#include "../PCH.h"

#include "Bindings.h"
#include "Common.h"
#include "Settings.h"

bool PapyrusSetSkillCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t value)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
    {
        SKSE::log::warn(
            "SetSkillCap: invalid skill slot {}",
            skillSlot);

        return false;
    }

    if (value < 1)
    {
        SKSE::log::warn(
            "SetSkillCap: invalid value {}",
            value);

        return false;
    }

    const bool result =
        Settings::SetSkillCap(
            static_cast<std::size_t>(
                skillSlot),
            static_cast<std::uint32_t>(
                value));

    SKSE::log::info(
        "Papyrus SetSkillCap({}, {}) -> {}",
        skillSlot,
        value,
        result);

    return result;
}

bool PapyrusSetFormulaCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot,
    std::int32_t value)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
    {
        SKSE::log::warn(
            "SetFormulaCap: invalid skill slot {}",
            skillSlot);

        return false;
    }

    if (value < 1)
    {
        SKSE::log::warn(
            "SetFormulaCap: invalid value {}",
            value);

        return false;
    }

    const bool result =
        Settings::SetFormulaCap(
            static_cast<std::size_t>(
                skillSlot),
            static_cast<std::uint32_t>(
                value));

    SKSE::log::info(
        "Papyrus SetFormulaCap({}, {}) -> {}",
        skillSlot,
        value,
        result);

    return result;
}

std::int32_t PapyrusGetSkillCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
    {
        return -1;
    }

    return static_cast<std::int32_t>(
        Settings::GetSkillCap(
            static_cast<std::size_t>(
                skillSlot)));
}

std::int32_t PapyrusGetFormulaCap(
    RE::StaticFunctionTag *,
    std::int32_t skillSlot)
{
    if (!Papyrus::IsValidSkillSlot(skillSlot))
    {
        return -1;
    }

    return static_cast<std::int32_t>(
        Settings::GetFormulaCap(
            static_cast<std::size_t>(
                skillSlot)));
}
