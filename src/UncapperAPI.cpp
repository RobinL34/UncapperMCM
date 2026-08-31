#include "PCH.h"
#include "UncapperAPI.h"

namespace
{
    using GetIniSkillCapFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using GetIniFormulaCapFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using GetIniEnchantMagnitudeCapFn =
        std::uint32_t(__stdcall *)();

    using GetIniEnchantChargeCapFn =
        std::uint32_t(__stdcall *)();

    using GetIniEnchantUseLinearChargeFn =
        std::uint32_t(__stdcall *)();

    using SetSkillCapOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t value);

    using SetFormulaCapOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t value);

    using SetEnchantMagnitudeCapOverrideFn =
        bool(__stdcall *)(
            std::uint32_t value);

    using SetEnchantChargeCapOverrideFn =
        bool(__stdcall *)(
            std::uint32_t value);

    using SetEnchantUseLinearChargeOverrideFn =
        bool(__stdcall *)(
            std::uint32_t value);

    using ClearOverridesFn =
        void(__stdcall *)();

    // ---------------------------------------------------------------------
    // Skill XP function types
    // ---------------------------------------------------------------------

    using GetIniSkillExpMultiplierFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using SetSkillExpBaseOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t baseHundredths,
            std::uint32_t offsetHundredths);

    using GetIniSkillExpBreakpointCountFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using GetIniSkillExpBreakpointValueFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t index);

    using BeginSkillExpOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot);

    using SetSkillExpBreakpointFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t index,
            std::uint32_t level,
            std::uint32_t baseHundredths,
            std::uint32_t offsetHundredths);

    using CommitSkillExpOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t count);

    // ---------------------------------------------------------------------
    // Player Level XP function types
    // ---------------------------------------------------------------------

    using GetIniLevelExpMultiplierFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using SetLevelExpMultiplierOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t multiplierHundredths);

    using GetIniLevelExpBreakpointCountFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot);

    using GetIniLevelExpBreakpointValueFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t index);

    using BeginLevelExpOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot);

    using SetLevelExpBreakpointFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t index,
            std::uint32_t level,
            std::uint32_t multiplierHundredths);

    using CommitLevelExpOverrideFn =
        bool(__stdcall *)(
            std::uint32_t skillSlot,
            std::uint32_t count);

    // ---------------------------------------------------------------------
    // Perks at level up function types
    // ---------------------------------------------------------------------

    using GetIniPerksAtLevelUpBreakpointCountFn =
        std::uint32_t(__stdcall *)();

    using GetIniPerksAtLevelUpBreakpointValueFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t index);

    using BeginPerksAtLevelUpOverrideFn =
        bool(__stdcall *)();

    using SetPerksAtLevelUpBreakpointFn =
        bool(__stdcall *)(
            std::uint32_t index,
            std::uint32_t level,
            std::uint32_t perkHundredths);

    using CommitPerksAtLevelUpOverrideFn =
        bool(__stdcall *)(
            std::uint32_t count);

    // ---------------------------------------------------------------------
    // Attributes at level up function types
    // ---------------------------------------------------------------------

    using GetIniAttributeBreakpointCountFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t tableIndex);

    using GetIniAttributeBreakpointValueFn =
        std::uint32_t(__stdcall *)(
            std::uint32_t tableIndex,
            std::uint32_t index);

    using BeginAttributeOverrideFn =
        bool(__stdcall *)(
            std::uint32_t tableIndex);

    using SetAttributeBreakpointFn =
        bool(__stdcall *)(
            std::uint32_t tableIndex,
            std::uint32_t index,
            std::uint32_t level,
            std::uint32_t value);

    using CommitAttributeOverrideFn =
        bool(__stdcall *)(
            std::uint32_t tableIndex,
            std::uint32_t count);

    using GetIniUseAttributesAtLevelUpFn =
        std::uint32_t(__stdcall *)();

    HMODULE GetUncapperModule()
    {
        return ::GetModuleHandleA(
            "SkyrimUncapper.dll");
    }

    template <class T>
    T GetUncapperFunction(const char *name)
    {
        const auto module = GetUncapperModule();

        if (!module)
        {
            return nullptr;
        }

        const auto address =
            ::GetProcAddress(module, name);

        if (!address)
        {
            return nullptr;
        }

        return reinterpret_cast<T>(address);
    }
}

namespace UncapperAPI
{
    bool IsAvailable()
    {
        return GetUncapperModule() != nullptr;
    }

    std::uint32_t GetIniSkillCap(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillCapFn>(
                "Uncapper_GetIniSkillCap");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillCap was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniFormulaCap(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniFormulaCapFn>(
                "Uncapper_GetIniFormulaCap");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniFormulaCap was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniEnchantMagnitudeCap()
    {
        const auto getter =
            GetUncapperFunction<GetIniEnchantMagnitudeCapFn>(
                "Uncapper_GetIniEnchantMagnitudeCap");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniEnchantMagnitudeCap was not found.");

            return UINT32_MAX;
        }

        return getter();
    }

    std::uint32_t GetIniEnchantChargeCap()
    {
        const auto getter =
            GetUncapperFunction<GetIniEnchantChargeCapFn>(
                "Uncapper_GetIniEnchantChargeCap");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniEnchantChargeCap was not found.");

            return UINT32_MAX;
        }

        return getter();
    }

    bool GetIniEnchantUseLinearCharge()
    {
        const auto getter =
            GetUncapperFunction<GetIniEnchantUseLinearChargeFn>(
                "Uncapper_GetIniEnchantUseLinearCharge");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniEnchantUseLinearCharge was not found.");

            return false;
        }

        return getter() != 0;
    }

    bool SetSkillCap(
        std::uint32_t skillSlot,
        std::uint32_t value)
    {
        const auto setter =
            GetUncapperFunction<SetSkillCapOverrideFn>(
                "Uncapper_SetSkillCapOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetSkillCapOverride was not found.");

            return false;
        }

        return setter(skillSlot, value);
    }

    bool SetFormulaCap(
        std::uint32_t skillSlot,
        std::uint32_t value)
    {
        const auto setter =
            GetUncapperFunction<SetFormulaCapOverrideFn>(
                "Uncapper_SetFormulaCapOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetFormulaCapOverride was not found.");

            return false;
        }

        return setter(skillSlot, value);
    }

    bool SetEnchantMagnitudeCap(
        std::uint32_t value)
    {
        const auto setter =
            GetUncapperFunction<SetEnchantMagnitudeCapOverrideFn>(
                "Uncapper_SetEnchantMagnitudeCapOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetEnchantMagnitudeCapOverride was not found.");

            return false;
        }

        return setter(value);
    }

    bool SetEnchantChargeCap(
        std::uint32_t value)
    {
        const auto setter =
            GetUncapperFunction<SetEnchantChargeCapOverrideFn>(
                "Uncapper_SetEnchantChargeCapOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetEnchantChargeCapOverride was not found.");

            return false;
        }

        return setter(value);
    }

    bool SetEnchantUseLinearCharge(
        bool enabled)
    {
        const auto setter =
            GetUncapperFunction<SetEnchantUseLinearChargeOverrideFn>(
                "Uncapper_SetEnchantUseLinearChargeOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetEnchantUseLinearChargeOverride was not found.");

            return false;
        }

        return setter(enabled ? 1u : 0u);
    }

    bool ClearOverrides()
    {
        const auto clear =
            GetUncapperFunction<ClearOverridesFn>(
                "Uncapper_ClearOverrides");

        if (!clear)
        {
            SKSE::log::error(
                "Uncapper_ClearOverrides was not found.");

            return false;
        }

        clear();

        return true;
    }

    // ---------------------------------------------------------------------
    // Skill XP - base multipliers
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpBaseMultiplier(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpMultiplierFn>(
                "Uncapper_GetIniSkillExpBaseMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpBaseMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniSkillExpOffsetMultiplier(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpMultiplierFn>(
                "Uncapper_GetIniSkillExpOffsetMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpOffsetMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    bool SetSkillExpBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths)
    {
        const auto setter =
            GetUncapperFunction<SetSkillExpBaseOverrideFn>(
                "Uncapper_SetSkillExpBaseOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetSkillExpBaseOverride was not found.");

            return false;
        }

        return setter(
            skillSlot,
            baseHundredths,
            offsetHundredths);
    }

    // ---------------------------------------------------------------------
    // Skill XP - INI breakpoints by base skill level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpSkillLevelBreakpointCount(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointCountFn>(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniSkillExpSkillLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    std::uint32_t GetIniSkillExpSkillLevelBreakpointBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointBaseMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointBaseMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    std::uint32_t GetIniSkillExpSkillLevelBreakpointOffsetMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointOffsetMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpSkillLevelBreakpointOffsetMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    bool BeginSkillExpSkillLevelOverride(
        std::uint32_t skillSlot)
    {
        const auto fn =
            GetUncapperFunction<BeginSkillExpOverrideFn>(
                "Uncapper_BeginSkillExpSkillLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginSkillExpSkillLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot);
    }

    bool SetSkillExpSkillLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths)
    {
        const auto fn =
            GetUncapperFunction<SetSkillExpBreakpointFn>(
                "Uncapper_SetSkillExpSkillLevelBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetSkillExpSkillLevelBreakpoint was not found.");

            return false;
        }

        return fn(
            skillSlot,
            index,
            level,
            baseHundredths,
            offsetHundredths);
    }

    bool CommitSkillExpSkillLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitSkillExpOverrideFn>(
                "Uncapper_CommitSkillExpSkillLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitSkillExpSkillLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot, count);
    }

    // ---------------------------------------------------------------------
    // Skill XP - INI breakpoints by character level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointCount(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointCountFn>(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointBaseMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointBaseMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointBaseMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    std::uint32_t GetIniSkillExpCharacterLevelBreakpointOffsetMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniSkillExpBreakpointValueFn>(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointOffsetMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniSkillExpCharacterLevelBreakpointOffsetMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot, index);
    }

    bool BeginSkillExpCharacterLevelOverride(
        std::uint32_t skillSlot)
    {
        const auto fn =
            GetUncapperFunction<BeginSkillExpOverrideFn>(
                "Uncapper_BeginSkillExpCharacterLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginSkillExpCharacterLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot);
    }

    bool SetSkillExpCharacterLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t baseHundredths,
        std::uint32_t offsetHundredths)
    {
        const auto fn =
            GetUncapperFunction<SetSkillExpBreakpointFn>(
                "Uncapper_SetSkillExpCharacterLevelBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetSkillExpCharacterLevelBreakpoint was not found.");

            return false;
        }

        return fn(
            skillSlot,
            index,
            level,
            baseHundredths,
            offsetHundredths);
    }

    bool CommitSkillExpCharacterLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitSkillExpOverrideFn>(
                "Uncapper_CommitSkillExpCharacterLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitSkillExpCharacterLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot, count);
    }
    // ---------------------------------------------------------------------
    // Player Level XP - base multiplier
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpMultiplier(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpMultiplierFn>(
                "Uncapper_GetIniLevelExpMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    bool SetLevelExpMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t multiplierHundredths)
    {
        const auto setter =
            GetUncapperFunction<SetLevelExpMultiplierOverrideFn>(
                "Uncapper_SetLevelExpMultiplierOverride");

        if (!setter)
        {
            SKSE::log::error(
                "Uncapper_SetLevelExpMultiplierOverride was not found.");

            return false;
        }

        return setter(
            skillSlot,
            multiplierHundredths);
    }

    // ---------------------------------------------------------------------
    // Player Level XP - INI breakpoints by base skill level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpSkillLevelBreakpointCount(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointCountFn>(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniLevelExpSkillLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointValueFn>(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(
            skillSlot,
            index);
    }

    std::uint32_t GetIniLevelExpSkillLevelBreakpointMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointValueFn>(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpSkillLevelBreakpointMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(
            skillSlot,
            index);
    }

    bool BeginLevelExpSkillLevelOverride(
        std::uint32_t skillSlot)
    {
        const auto fn =
            GetUncapperFunction<BeginLevelExpOverrideFn>(
                "Uncapper_BeginLevelExpSkillLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginLevelExpSkillLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot);
    }

    bool SetLevelExpSkillLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t multiplierHundredths)
    {
        const auto fn =
            GetUncapperFunction<SetLevelExpBreakpointFn>(
                "Uncapper_SetLevelExpSkillLevelBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetLevelExpSkillLevelBreakpoint was not found.");

            return false;
        }

        return fn(
            skillSlot,
            index,
            level,
            multiplierHundredths);
    }

    bool CommitLevelExpSkillLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitLevelExpOverrideFn>(
                "Uncapper_CommitLevelExpSkillLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitLevelExpSkillLevelOverride was not found.");

            return false;
        }

        return fn(
            skillSlot,
            count);
    }

    // ---------------------------------------------------------------------
    // Player Level XP - INI breakpoints by character level
    // ---------------------------------------------------------------------

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointCount(
        std::uint32_t skillSlot)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointCountFn>(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter(skillSlot);
    }

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointLevel(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointValueFn>(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(
            skillSlot,
            index);
    }

    std::uint32_t GetIniLevelExpCharacterLevelBreakpointMultiplier(
        std::uint32_t skillSlot,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniLevelExpBreakpointValueFn>(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointMultiplier");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniLevelExpCharacterLevelBreakpointMultiplier was not found.");

            return UINT32_MAX;
        }

        return getter(
            skillSlot,
            index);
    }

    bool BeginLevelExpCharacterLevelOverride(
        std::uint32_t skillSlot)
    {
        const auto fn =
            GetUncapperFunction<BeginLevelExpOverrideFn>(
                "Uncapper_BeginLevelExpCharacterLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginLevelExpCharacterLevelOverride was not found.");

            return false;
        }

        return fn(skillSlot);
    }

    bool SetLevelExpCharacterLevelBreakpoint(
        std::uint32_t skillSlot,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t multiplierHundredths)
    {
        const auto fn =
            GetUncapperFunction<SetLevelExpBreakpointFn>(
                "Uncapper_SetLevelExpCharacterLevelBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetLevelExpCharacterLevelBreakpoint was not found.");

            return false;
        }

        return fn(
            skillSlot,
            index,
            level,
            multiplierHundredths);
    }

    bool CommitLevelExpCharacterLevelOverride(
        std::uint32_t skillSlot,
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitLevelExpOverrideFn>(
                "Uncapper_CommitLevelExpCharacterLevelOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitLevelExpCharacterLevelOverride was not found.");

            return false;
        }

        return fn(
            skillSlot,
            count);
    }

    // ---------------------------------------------------------------------
    // Perks at level up
    // ---------------------------------------------------------------------

    std::uint32_t GetIniPerksAtLevelUpBreakpointCount()
    {
        const auto getter =
            GetUncapperFunction<GetIniPerksAtLevelUpBreakpointCountFn>(
                "Uncapper_GetIniPerksAtLevelUpBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniPerksAtLevelUpBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter();
    }

    std::uint32_t GetIniPerksAtLevelUpBreakpointLevel(
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniPerksAtLevelUpBreakpointValueFn>(
                "Uncapper_GetIniPerksAtLevelUpBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniPerksAtLevelUpBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(index);
    }

    std::uint32_t GetIniPerksAtLevelUpBreakpointValue(
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniPerksAtLevelUpBreakpointValueFn>(
                "Uncapper_GetIniPerksAtLevelUpBreakpointValue");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniPerksAtLevelUpBreakpointValue was not found.");

            return UINT32_MAX;
        }

        return getter(index);
    }

    bool BeginPerksAtLevelUpOverride()
    {
        const auto fn =
            GetUncapperFunction<BeginPerksAtLevelUpOverrideFn>(
                "Uncapper_BeginPerksAtLevelUpOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginPerksAtLevelUpOverride was not found.");

            return false;
        }

        return fn();
    }

    bool SetPerksAtLevelUpBreakpoint(
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t perkHundredths)
    {
        const auto fn =
            GetUncapperFunction<SetPerksAtLevelUpBreakpointFn>(
                "Uncapper_SetPerksAtLevelUpBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetPerksAtLevelUpBreakpoint was not found.");

            return false;
        }

        return fn(
            index,
            level,
            perkHundredths);
    }

    bool CommitPerksAtLevelUpOverride(
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitPerksAtLevelUpOverrideFn>(
                "Uncapper_CommitPerksAtLevelUpOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitPerksAtLevelUpOverride was not found.");

            return false;
        }

        return fn(count);
    }

    // ---------------------------------------------------------------------
    // Attributes at level up
    // ---------------------------------------------------------------------

    std::uint32_t GetIniAttributeBreakpointCount(
        std::uint32_t tableIndex)
    {
        const auto getter =
            GetUncapperFunction<GetIniAttributeBreakpointCountFn>(
                "Uncapper_GetIniAttributeBreakpointCount");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniAttributeBreakpointCount was not found.");

            return UINT32_MAX;
        }

        return getter(tableIndex);
    }

    std::uint32_t GetIniAttributeBreakpointLevel(
        std::uint32_t tableIndex,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniAttributeBreakpointValueFn>(
                "Uncapper_GetIniAttributeBreakpointLevel");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniAttributeBreakpointLevel was not found.");

            return UINT32_MAX;
        }

        return getter(
            tableIndex,
            index);
    }

    std::uint32_t GetIniAttributeBreakpointValue(
        std::uint32_t tableIndex,
        std::uint32_t index)
    {
        const auto getter =
            GetUncapperFunction<GetIniAttributeBreakpointValueFn>(
                "Uncapper_GetIniAttributeBreakpointValue");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniAttributeBreakpointValue was not found.");

            return UINT32_MAX;
        }

        return getter(
            tableIndex,
            index);
    }

    bool BeginAttributeOverride(
        std::uint32_t tableIndex)
    {
        const auto fn =
            GetUncapperFunction<BeginAttributeOverrideFn>(
                "Uncapper_BeginAttributeOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_BeginAttributeOverride was not found.");

            return false;
        }

        return fn(tableIndex);
    }

    bool SetAttributeBreakpoint(
        std::uint32_t tableIndex,
        std::uint32_t index,
        std::uint32_t level,
        std::uint32_t value)
    {
        const auto fn =
            GetUncapperFunction<SetAttributeBreakpointFn>(
                "Uncapper_SetAttributeBreakpoint");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_SetAttributeBreakpoint was not found.");

            return false;
        }

        return fn(
            tableIndex,
            index,
            level,
            value);
    }

    bool CommitAttributeOverride(
        std::uint32_t tableIndex,
        std::uint32_t count)
    {
        const auto fn =
            GetUncapperFunction<CommitAttributeOverrideFn>(
                "Uncapper_CommitAttributeOverride");

        if (!fn)
        {
            SKSE::log::error(
                "Uncapper_CommitAttributeOverride was not found.");

            return false;
        }

        return fn(
            tableIndex,
            count);
    }

    std::uint32_t GetIniUseAttributesAtLevelUp()
    {
        const auto getter =
            GetUncapperFunction<GetIniUseAttributesAtLevelUpFn>(
                "Uncapper_GetIniUseAttributesAtLevelUp");

        if (!getter)
        {
            SKSE::log::error(
                "Uncapper_GetIniUseAttributesAtLevelUp was not found.");

            return UINT32_MAX;
        }

        return getter();
    }
}
