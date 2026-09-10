#include "../PCH.h"

#include "Bindings.h"
#include "Settings.h"

bool PapyrusSetEnchantMagnitudeCap(
    RE::StaticFunctionTag *,
    std::int32_t value)
{
    if (value < 1)
    {
        SKSE::log::warn(
            "SetEnchantMagnitudeCap: invalid value {}",
            value);

        return false;
    }

    const bool result =
        Settings::SetEnchantMagnitudeCap(
            static_cast<std::uint32_t>(
                value));

    SKSE::log::info(
        "Papyrus SetEnchantMagnitudeCap({}) -> {}",
        value,
        result);

    return result;
}

bool PapyrusSetEnchantChargeCap(
    RE::StaticFunctionTag *,
    std::int32_t value)
{
    if (value < 1)
    {
        SKSE::log::warn(
            "SetEnchantChargeCap: invalid value {}",
            value);

        return false;
    }

    const bool result =
        Settings::SetEnchantChargeCap(
            static_cast<std::uint32_t>(
                value));

    SKSE::log::info(
        "Papyrus SetEnchantChargeCap({}) -> {}",
        value,
        result);

    return result;
}

void PapyrusSetEnchantUseLinearCharge(
    RE::StaticFunctionTag *,
    bool enabled)
{
    const bool result =
        Settings::SetEnchantUseLinearCharge(
            enabled);

    SKSE::log::info(
        "Papyrus SetEnchantUseLinearCharge({}) -> {}",
        enabled,
        result);
}

std::int32_t PapyrusGetEnchantMagnitudeCap(
    RE::StaticFunctionTag *)
{
    return static_cast<std::int32_t>(
        Settings::GetEnchantMagnitudeCap());
}

std::int32_t PapyrusGetEnchantChargeCap(
    RE::StaticFunctionTag *)
{
    return static_cast<std::int32_t>(
        Settings::GetEnchantChargeCap());
}

bool PapyrusGetEnchantUseLinearCharge(
    RE::StaticFunctionTag *)
{
    return Settings::
        GetEnchantUseLinearCharge();
}

