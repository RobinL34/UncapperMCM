#include "PCH.h"

#include "Serialization.h"
#include "Settings.h"

namespace
{
    constexpr std::uint32_t SERIALIZATION_ID = 'UCMC';
    constexpr std::uint32_t RECORD_SETTINGS = 'SETT';

    constexpr std::uint32_t RECORD_VERSION_V1 = 1;
    constexpr std::uint32_t RECORD_VERSION_V2 = 2;
    constexpr std::uint32_t RECORD_VERSION_V3 = 3;
    constexpr std::uint32_t RECORD_VERSION_V4 = 4;
    constexpr std::uint32_t RECORD_VERSION_V5 = 5;

    constexpr std::uint32_t CURRENT_RECORD_VERSION =
        RECORD_VERSION_V5;

    // -------------------------------------------------------------------------
    // Version 1
    // -------------------------------------------------------------------------

    struct SavedSettingsV1
    {
        bool enabled = false;

        std::array<std::uint32_t, Settings::SKILL_COUNT>
            skillCaps{};

        std::array<std::uint32_t, Settings::SKILL_COUNT>
            formulaCaps{};
    };

    // -------------------------------------------------------------------------
    // Version 2
    // -------------------------------------------------------------------------

    struct SavedSettingsV2
    {
        std::uint32_t enabled = 0;

        std::array<std::uint32_t, Settings::SKILL_COUNT>
            skillCaps{};

        std::array<std::uint32_t, Settings::SKILL_COUNT>
            formulaCaps{};

        std::uint32_t enchantMagnitudeCap = 0;
        std::uint32_t enchantChargeCap = 0;

        std::uint32_t enchantUseLinearCharge = 0;
    };

    // -------------------------------------------------------------------------
    // Version 3 serialized breakpoint.
    //
    // Explicit fixed-width representation.
    // std::vector itself is NEVER serialized.
    // -------------------------------------------------------------------------

    struct SavedSkillExpBreakpointV3
    {
        std::uint32_t level = 0;
        std::uint32_t baseHundredths = 100;
        std::uint32_t offsetHundredths = 100;
    };

    struct SavedMultiplierBreakpointV4
    {
        std::uint32_t level = 0;
        std::uint32_t multiplierHundredths = 100;
    };

    // -------------------------------------------------------------------------
    // Generic serialization helpers
    // -------------------------------------------------------------------------

    template <class T>
    bool WriteValue(
        SKSE::SerializationInterface *serialization,
        const T &value)
    {
        return serialization->WriteRecordData(
            value);
    }

    template <class T>
    bool ReadValue(
        SKSE::SerializationInterface *serialization,
        T &value,
        std::uint32_t &bytesReadTotal,
        std::uint32_t recordLength)
    {
        if (
            bytesReadTotal + sizeof(T) >
            recordLength)
        {
            return false;
        }

        const auto bytesRead =
            serialization->ReadRecordData(
                value);

        if (bytesRead != sizeof(T))
        {
            return false;
        }

        bytesReadTotal +=
            static_cast<std::uint32_t>(
                sizeof(T));

        return true;
    }

    // -------------------------------------------------------------------------
    // Write one V3 Skill XP breakpoint table.
    // -------------------------------------------------------------------------

    bool WriteSkillExpBreakpointTable(
        SKSE::SerializationInterface *serialization,
        const std::vector<Settings::SkillExpBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_SKILL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Cannot serialize invalid Skill XP "
                "breakpoint count {}.",
                breakpoints.size());

            return false;
        }

                    if (breakpoints.front().level != 0)
            {
                SKSE::log::error(
                    "Cannot serialize Skill XP breakpoint table "
                    "without a level 0 breakpoint.");

                return false;
            }

        const auto count =
            static_cast<std::uint32_t>(
                breakpoints.size());

        if (
            !WriteValue(
                serialization,
                count))
        {
            return false;
        }

        for (
            const auto &breakpoint :
            breakpoints)
        {
            SavedSkillExpBreakpointV3 saved{};

            saved.level =
                breakpoint.level;

            saved.baseHundredths =
                breakpoint.baseHundredths;

            saved.offsetHundredths =
                breakpoint.offsetHundredths;

            if (
                !WriteValue(
                    serialization,
                    saved))
            {
                return false;
            }
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Read one V3 Skill XP breakpoint table.
    // -------------------------------------------------------------------------

    bool ReadSkillExpBreakpointTable(
        SKSE::SerializationInterface *serialization,
        std::uint32_t &bytesReadTotal,
        std::uint32_t recordLength,
        std::vector<Settings::SkillExpBreakpoint> &output)
    {
        std::uint32_t count = 0;

        if (
            !ReadValue(
                serialization,
                count,
                bytesReadTotal,
                recordLength))
        {
            return false;
        }

        if (
            count == 0 ||
            count >
                Settings::MAX_SKILL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid serialized Skill XP "
                "breakpoint count {}.",
                count);

            return false;
        }

        output.clear();
        output.reserve(count);

        std::uint32_t previousLevel = 0;

        for (
            std::uint32_t i = 0;
            i < count;
            ++i)
        {
            SavedSkillExpBreakpointV3 saved{};

            if (
                !ReadValue(
                    serialization,
                    saved,
                    bytesReadTotal,
                    recordLength))
            {
                return false;
            }

            if (
                saved.level >
                Settings::MAX_BREAKPOINT_LEVEL)
            {
                SKSE::log::error(
                    "Invalid serialized breakpoint "
                    "level {}.",
                    saved.level);

                return false;
            }

            if (
                saved.baseHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS ||
                saved.offsetHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                SKSE::log::error(
                    "Invalid serialized Skill XP "
                    "multiplier.");

                return false;
            }

            if (
                i == 0 &&
                saved.level != 0)
            {
                SKSE::log::error(
                    "Serialized Skill XP breakpoint table "
                    "does not start at level 0.");

                return false;
            }

            if (
                i > 0 &&
                saved.level <= previousLevel)
            {
                SKSE::log::error(
                    "Serialized Skill XP breakpoint "
                    "table is not strictly sorted.");

                return false;
            }

            previousLevel =
                saved.level;

            output.push_back(
                Settings::SkillExpBreakpoint{
                    saved.level,
                    saved.baseHundredths,
                    saved.offsetHundredths});
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Write one V4 single-multiplier breakpoint table.
    // -------------------------------------------------------------------------

    bool WriteMultiplierBreakpointTable(
        SKSE::SerializationInterface *serialization,
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_LEVEL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Cannot serialize invalid Player Level XP "
                "breakpoint count {}.",
                breakpoints.size());

            return false;
        }

        if (breakpoints.front().level != 0)
        {
            SKSE::log::error(
                "Cannot serialize Player Level XP breakpoint "
                "table without a level 0 breakpoint.");

            return false;
        }

        const auto count =
            static_cast<std::uint32_t>(
                breakpoints.size());

        if (
            !WriteValue(
                serialization,
                count))
        {
            return false;
        }

        std::uint32_t previousLevel = 0;

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                breakpoint.level >
                    Settings::MAX_BREAKPOINT_LEVEL ||
                breakpoint.multiplierHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                i > 0 &&
                breakpoint.level <= previousLevel)
            {
                return false;
            }

            previousLevel =
                breakpoint.level;

            SavedMultiplierBreakpointV4 saved{};

            saved.level =
                breakpoint.level;

            saved.multiplierHundredths =
                breakpoint.multiplierHundredths;

            if (
                !WriteValue(
                    serialization,
                    saved))
            {
                return false;
            }
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Read one V4 single-multiplier breakpoint table.
    // -------------------------------------------------------------------------

    bool ReadMultiplierBreakpointTable(
        SKSE::SerializationInterface *serialization,
        std::uint32_t &bytesReadTotal,
        std::uint32_t recordLength,
        std::vector<Settings::MultiplierBreakpoint> &output)
    {
        std::uint32_t count = 0;

        if (
            !ReadValue(
                serialization,
                count,
                bytesReadTotal,
                recordLength))
        {
            return false;
        }

        if (
            count == 0 ||
            count >
                Settings::MAX_LEVEL_EXP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid serialized Player Level XP "
                "breakpoint count {}.",
                count);

            return false;
        }

        output.clear();
        output.reserve(count);

        std::uint32_t previousLevel = 0;

        for (
            std::uint32_t i = 0;
            i < count;
            ++i)
        {
            SavedMultiplierBreakpointV4 saved{};

            if (
                !ReadValue(
                    serialization,
                    saved,
                    bytesReadTotal,
                    recordLength))
            {
                return false;
            }

            if (
                i == 0 &&
                saved.level != 0)
            {
                SKSE::log::error(
                    "Serialized Player Level XP breakpoint "
                    "table does not start at level 0.");

                return false;
            }

            if (
                saved.level >
                Settings::MAX_BREAKPOINT_LEVEL)
            {
                return false;
            }

            if (
                saved.multiplierHundredths >
                Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                i > 0 &&
                saved.level <= previousLevel)
            {
                SKSE::log::error(
                    "Serialized Player Level XP breakpoint "
                    "table is not strictly sorted.");

                return false;
            }

            previousLevel =
                saved.level;

            output.push_back(
                Settings::MultiplierBreakpoint{
                    saved.level,
                    saved.multiplierHundredths});
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Write the V5 PerksAtLevelUp breakpoint table.
    // -------------------------------------------------------------------------

    bool WritePerksAtLevelUpBreakpointTable(
        SKSE::SerializationInterface *serialization,
        const std::vector<Settings::MultiplierBreakpoint> &breakpoints)
    {
        if (
            breakpoints.empty() ||
            breakpoints.size() >
                Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Cannot serialize invalid PerksAtLevelUp "
                "breakpoint count {}.",
                breakpoints.size());

            return false;
        }

        if (breakpoints.front().level != 0)
        {
            SKSE::log::error(
                "Cannot serialize PerksAtLevelUp breakpoint "
                "table without a level 0 breakpoint.");

            return false;
        }

        const auto count =
            static_cast<std::uint32_t>(
                breakpoints.size());

        if (
            !WriteValue(
                serialization,
                count))
        {
            return false;
        }

        std::uint32_t previousLevel = 0;

        for (
            std::size_t i = 0;
            i < breakpoints.size();
            ++i)
        {
            const auto &breakpoint =
                breakpoints[i];

            if (
                breakpoint.level >
                    Settings::MAX_BREAKPOINT_LEVEL ||
                breakpoint.multiplierHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                i > 0 &&
                breakpoint.level <= previousLevel)
            {
                return false;
            }

            previousLevel =
                breakpoint.level;

            SavedMultiplierBreakpointV4 saved{};

            saved.level =
                breakpoint.level;

            saved.multiplierHundredths =
                breakpoint.multiplierHundredths;

            if (
                !WriteValue(
                    serialization,
                    saved))
            {
                return false;
            }
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Read the V5 PerksAtLevelUp breakpoint table.
    // -------------------------------------------------------------------------

    bool ReadPerksAtLevelUpBreakpointTable(
        SKSE::SerializationInterface *serialization,
        std::uint32_t &bytesReadTotal,
        std::uint32_t recordLength,
        std::vector<Settings::MultiplierBreakpoint> &output)
    {
        std::uint32_t count = 0;

        if (
            !ReadValue(
                serialization,
                count,
                bytesReadTotal,
                recordLength))
        {
            return false;
        }

        if (
            count == 0 ||
            count >
                Settings::MAX_PERKS_AT_LEVEL_UP_BREAKPOINTS)
        {
            SKSE::log::error(
                "Invalid serialized PerksAtLevelUp "
                "breakpoint count {}.",
                count);

            return false;
        }

        output.clear();
        output.reserve(count);

        std::uint32_t previousLevel = 0;

        for (
            std::uint32_t i = 0;
            i < count;
            ++i)
        {
            SavedMultiplierBreakpointV4 saved{};

            if (
                !ReadValue(
                    serialization,
                    saved,
                    bytesReadTotal,
                    recordLength))
            {
                return false;
            }

            if (
                i == 0 &&
                saved.level != 0)
            {
                SKSE::log::error(
                    "Serialized PerksAtLevelUp breakpoint "
                    "table does not start at level 0.");

                return false;
            }

            if (
                saved.level >
                    Settings::MAX_BREAKPOINT_LEVEL ||
                saved.multiplierHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                i > 0 &&
                saved.level <= previousLevel)
            {
                SKSE::log::error(
                    "Serialized PerksAtLevelUp breakpoint "
                    "table is not strictly sorted.");

                return false;
            }

            previousLevel =
                saved.level;

            output.push_back(
                Settings::MultiplierBreakpoint{
                    saved.level,
                    saved.multiplierHundredths});
        }

        return true;
    }

    // -------------------------------------------------------------------------
    // Save V5
    // -------------------------------------------------------------------------

    void SaveCallback(
        SKSE::SerializationInterface *serialization)
    {
        const auto &settings =
            Settings::Get();

        if (
            !serialization->OpenRecord(
                RECORD_SETTINGS,
                CURRENT_RECORD_VERSION))
        {
            SKSE::log::error(
                "Failed to open UncapperMCM "
                "serialization record.");

            return;
        }

        const std::uint32_t enabled =
            settings.enabled ? 1u : 0u;

        if (
            !WriteValue(
                serialization,
                enabled))
        {
            goto save_failed;
        }

        // -------------------------------------------------------------
        // Caps
        // -------------------------------------------------------------

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !WriteValue(
                    serialization,
                    settings.skillCaps[i]))
            {
                goto save_failed;
            }
        }

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !WriteValue(
                    serialization,
                    settings.formulaCaps[i]))
            {
                goto save_failed;
            }
        }

        // -------------------------------------------------------------
        // Enchanting
        // -------------------------------------------------------------

        if (
            !WriteValue(
                serialization,
                settings.enchanting.magnitudeLevelCap))
        {
            goto save_failed;
        }

        if (
            !WriteValue(
                serialization,
                settings.enchanting.chargeLevelCap))
        {
            goto save_failed;
        }

        {
            const std::uint32_t linearCharge =
                settings.enchanting.useLinearChargeFormula
                    ? 1u
                    : 0u;

            if (
                !WriteValue(
                    serialization,
                    linearCharge))
            {
                goto save_failed;
            }
        }

        // -------------------------------------------------------------
        // SkillExpGainMults
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            const auto &skillExp =
                settings.skillExpGain[skillSlot];

            if (
                !WriteValue(
                    serialization,
                    skillExp.baseHundredths))
            {
                goto save_failed;
            }

            if (
                !WriteValue(
                    serialization,
                    skillExp.offsetHundredths))
            {
                goto save_failed;
            }

            if (
                !WriteSkillExpBreakpointTable(
                    serialization,
                    skillExp.bySkillLevel))
            {
                goto save_failed;
            }

            if (
                !WriteSkillExpBreakpointTable(
                    serialization,
                    skillExp.byCharacterLevel))
            {
                goto save_failed;
            }
        }

        // -------------------------------------------------------------
        // LevelSkillExpMults / Player Level XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            const auto &levelExp =
                settings.playerLevelExp[skillSlot];

            if (
                !WriteValue(
                    serialization,
                    levelExp.multiplierHundredths))
            {
                goto save_failed;
            }

            if (
                !WriteMultiplierBreakpointTable(
                    serialization,
                    levelExp.bySkillLevel))
            {
                goto save_failed;
            }

            if (
                !WriteMultiplierBreakpointTable(
                    serialization,
                    levelExp.byCharacterLevel))
            {
                goto save_failed;
            }
        }

        // -------------------------------------------------------------
        // PerksAtLevelUp
        // -------------------------------------------------------------

        if (
            !WritePerksAtLevelUpBreakpointTable(
                serialization,
                settings.perksAtLevelUp))
        {
            goto save_failed;
        }

        SKSE::log::info(
            "UncapperMCM settings saved "
            "(serialization version {}).",
            CURRENT_RECORD_VERSION);

        return;

    save_failed:

        SKSE::log::error(
            "Failed to save UncapperMCM "
            "serialization V5 record.");
    }

    // -------------------------------------------------------------------------
    // Load V1
    // -------------------------------------------------------------------------

    bool LoadVersion1(
        SKSE::SerializationInterface *serialization,
        std::uint32_t length)
    {
        if (
            length != sizeof(SavedSettingsV1))
        {
            SKSE::log::error(
                "Invalid UncapperMCM V1 "
                "save record size.");

            return false;
        }

        SavedSettingsV1 saved{};

        const auto bytesRead =
            serialization->ReadRecordData(
                saved);

        if (
            bytesRead != sizeof(SavedSettingsV1))
        {
            SKSE::log::error(
                "Failed to read UncapperMCM "
                "V1 save record.");

            return false;
        }

        auto &settings =
            Settings::Get();

        settings.enabled =
            saved.enabled;

        settings.skillCaps =
            saved.skillCaps;

        settings.formulaCaps =
            saved.formulaCaps;

        // Enchanting + Skill XP + Player Level XP remain at the INI values.
        // loaded by Settings::Reset().
        SKSE::log::info(
            "Migrating UncapperMCM serialization "
            "V1 -> V5. Enchanting, Skill XP, "
            "Player Level XP and PerksAtLevelUp will use "
            "SkyrimUncapper.ini values.");

        return true;
    }

    // -------------------------------------------------------------------------
    // Load V2
    // -------------------------------------------------------------------------

    bool LoadVersion2(
        SKSE::SerializationInterface *serialization,
        std::uint32_t length)
    {
        if (
            length != sizeof(SavedSettingsV2))
        {
            SKSE::log::error(
                "Invalid UncapperMCM V2 "
                "save record size.");

            return false;
        }

        SavedSettingsV2 saved{};

        const auto bytesRead =
            serialization->ReadRecordData(
                saved);

        if (
            bytesRead != sizeof(SavedSettingsV2))
        {
            SKSE::log::error(
                "Failed to read UncapperMCM "
                "V2 save record.");

            return false;
        }

        auto &settings =
            Settings::Get();

        settings.enabled =
            saved.enabled != 0;

        settings.skillCaps =
            saved.skillCaps;

        settings.formulaCaps =
            saved.formulaCaps;

        settings.enchanting.magnitudeLevelCap =
            saved.enchantMagnitudeCap;

        settings.enchanting.chargeLevelCap =
            saved.enchantChargeCap;

        settings.enchanting.useLinearChargeFormula =
            saved.enchantUseLinearCharge != 0;

        // Skill XP remains at the current INI baseline.
        SKSE::log::info(
            "Migrating UncapperMCM serialization "
            "V2 -> V5. Skill XP, Player Level XP and "
            "PerksAtLevelUp "
            "will use SkyrimUncapper.ini values.");

        return true;
    }

    // -------------------------------------------------------------------------
    // Load V3
    // -------------------------------------------------------------------------

    bool LoadVersion3(
        SKSE::SerializationInterface *serialization,
        std::uint32_t length)
    {
        std::uint32_t bytesReadTotal = 0;

        auto &settings =
            Settings::Get();

        // -------------------------------------------------------------
        // Enabled
        // -------------------------------------------------------------

        std::uint32_t enabled = 0;

        if (
            !ReadValue(
                serialization,
                enabled,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (
            enabled > 1)
        {
            return false;
        }

        // -------------------------------------------------------------
        // Read into temporary storage first.
        // -------------------------------------------------------------

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            skillCaps{};

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            formulaCaps{};

        std::array<
            Settings::SkillExpSettings,
            Settings::SKILL_COUNT>
            skillExpGain{};

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    skillCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillCaps[i] < Settings::MIN_CAP ||
                skillCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    formulaCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                formulaCaps[i] < Settings::MIN_CAP ||
                formulaCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Enchanting
        // -------------------------------------------------------------

        std::uint32_t magnitudeCap = 0;
        std::uint32_t chargeCap = 0;
        std::uint32_t linearCharge = 0;

        if (
            !ReadValue(
                serialization,
                magnitudeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                chargeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                linearCharge,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (
            magnitudeCap < Settings::MIN_CAP ||
            magnitudeCap > Settings::HARD_MAX_CAP)
        {
            return false;
        }

        if (
            chargeCap < Settings::MIN_CAP ||
            chargeCap >
                Settings::MAX_ENCHANTING_CHARGE_CAP)
        {
            return false;
        }

        if (
            linearCharge > 1)
        {
            return false;
        }

        // -------------------------------------------------------------
        // Skill XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            auto &skillExp =
                skillExpGain[skillSlot];

            if (
                !ReadValue(
                    serialization,
                    skillExp.baseHundredths,
                    bytesReadTotal,
                    length) ||
                !ReadValue(
                    serialization,
                    skillExp.offsetHundredths,
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillExp.baseHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS ||
                skillExp.offsetHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.bySkillLevel))
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.byCharacterLevel))
            {
                return false;
            }
        }

        // There should be no unexpected trailing bytes.
        if (
            bytesReadTotal != length)
        {
            SKSE::log::error(
                "Unexpected UncapperMCM V3 "
                "record size. Read {}, expected {}.",
                bytesReadTotal,
                length);

            return false;
        }

        // -------------------------------------------------------------
        // Commit only after the complete record was validated.
        // -------------------------------------------------------------

        settings.enabled =
            enabled != 0;

        settings.skillCaps =
            skillCaps;

        settings.formulaCaps =
            formulaCaps;

        settings.enchanting.magnitudeLevelCap =
            magnitudeCap;

        settings.enchanting.chargeLevelCap =
            chargeCap;

        settings.enchanting.useLinearChargeFormula =
            linearCharge != 0;

        settings.skillExpGain =
            std::move(skillExpGain);

        SKSE::log::info(
            "Migrating UncapperMCM serialization "
            "V3 -> V5. Player Level XP and PerksAtLevelUp "
            "will use SkyrimUncapper.ini values.");

        return true;
    }

    // -------------------------------------------------------------------------
    // Load V4
    // -------------------------------------------------------------------------

    bool LoadVersion4(
        SKSE::SerializationInterface *serialization,
        std::uint32_t length)
    {
        std::uint32_t bytesReadTotal = 0;

        auto &settings =
            Settings::Get();

        std::uint32_t enabled = 0;

        if (
            !ReadValue(
                serialization,
                enabled,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (enabled > 1)
        {
            return false;
        }

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            skillCaps{};

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            formulaCaps{};

        std::array<
            Settings::SkillExpSettings,
            Settings::SKILL_COUNT>
            skillExpGain{};

        std::array<
            Settings::PlayerLevelExpSettings,
            Settings::SKILL_COUNT>
            playerLevelExp{};

        // -------------------------------------------------------------
        // Caps
        // -------------------------------------------------------------

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    skillCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillCaps[i] < Settings::MIN_CAP ||
                skillCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    formulaCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                formulaCaps[i] < Settings::MIN_CAP ||
                formulaCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Enchanting
        // -------------------------------------------------------------

        std::uint32_t magnitudeCap = 0;
        std::uint32_t chargeCap = 0;
        std::uint32_t linearCharge = 0;

        if (
            !ReadValue(
                serialization,
                magnitudeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                chargeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                linearCharge,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (
            magnitudeCap < Settings::MIN_CAP ||
            magnitudeCap > Settings::HARD_MAX_CAP)
        {
            return false;
        }

        if (
            chargeCap < Settings::MIN_CAP ||
            chargeCap >
                Settings::MAX_ENCHANTING_CHARGE_CAP)
        {
            return false;
        }

        if (linearCharge > 1)
        {
            return false;
        }

        // -------------------------------------------------------------
        // Skill XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            auto &skillExp =
                skillExpGain[skillSlot];

            if (
                !ReadValue(
                    serialization,
                    skillExp.baseHundredths,
                    bytesReadTotal,
                    length) ||
                !ReadValue(
                    serialization,
                    skillExp.offsetHundredths,
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillExp.baseHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS ||
                skillExp.offsetHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.bySkillLevel))
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.byCharacterLevel))
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Player Level XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            auto &levelExp =
                playerLevelExp[skillSlot];

            if (
                !ReadValue(
                    serialization,
                    levelExp.multiplierHundredths,
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                levelExp.multiplierHundredths >
                Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                !ReadMultiplierBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    levelExp.bySkillLevel))
            {
                return false;
            }

            if (
                !ReadMultiplierBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    levelExp.byCharacterLevel))
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Exact record-size validation
        // -------------------------------------------------------------

        if (
            bytesReadTotal != length)
        {
            SKSE::log::error(
                "Unexpected UncapperMCM V4 "
                "record size. Read {}, expected {}.",
                bytesReadTotal,
                length);

            return false;
        }

        // -------------------------------------------------------------
        // Commit only once everything has been validated.
        // -------------------------------------------------------------

        settings.enabled =
            enabled != 0;

        settings.skillCaps =
            skillCaps;

        settings.formulaCaps =
            formulaCaps;

        settings.enchanting.magnitudeLevelCap =
            magnitudeCap;

        settings.enchanting.chargeLevelCap =
            chargeCap;

        settings.enchanting.useLinearChargeFormula =
            linearCharge != 0;

        settings.skillExpGain =
            std::move(skillExpGain);

        settings.playerLevelExp =
            std::move(playerLevelExp);

        SKSE::log::info(
            "Migrating UncapperMCM serialization "
            "V4 -> V5. PerksAtLevelUp will use "
            "SkyrimUncapper.ini values.");

        return true;
    }

    // -------------------------------------------------------------------------
    // Load V5
    // -------------------------------------------------------------------------

    bool LoadVersion5(
        SKSE::SerializationInterface *serialization,
        std::uint32_t length)
    {
        std::uint32_t bytesReadTotal = 0;

        auto &settings =
            Settings::Get();

        std::uint32_t enabled = 0;

        if (
            !ReadValue(
                serialization,
                enabled,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (enabled > 1)
        {
            return false;
        }

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            skillCaps{};

        std::array<
            std::uint32_t,
            Settings::SKILL_COUNT>
            formulaCaps{};

        std::array<
            Settings::SkillExpSettings,
            Settings::SKILL_COUNT>
            skillExpGain{};

        std::array<
            Settings::PlayerLevelExpSettings,
            Settings::SKILL_COUNT>
            playerLevelExp{};

        std::vector<Settings::MultiplierBreakpoint>
            perksAtLevelUp;

        // -------------------------------------------------------------
        // Caps
        // -------------------------------------------------------------

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    skillCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillCaps[i] < Settings::MIN_CAP ||
                skillCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        for (
            std::size_t i = 0;
            i < Settings::SKILL_COUNT;
            ++i)
        {
            if (
                !ReadValue(
                    serialization,
                    formulaCaps[i],
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                formulaCaps[i] < Settings::MIN_CAP ||
                formulaCaps[i] > Settings::HARD_MAX_CAP)
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Enchanting
        // -------------------------------------------------------------

        std::uint32_t magnitudeCap = 0;
        std::uint32_t chargeCap = 0;
        std::uint32_t linearCharge = 0;

        if (
            !ReadValue(
                serialization,
                magnitudeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                chargeCap,
                bytesReadTotal,
                length) ||
            !ReadValue(
                serialization,
                linearCharge,
                bytesReadTotal,
                length))
        {
            return false;
        }

        if (
            magnitudeCap < Settings::MIN_CAP ||
            magnitudeCap > Settings::HARD_MAX_CAP)
        {
            return false;
        }

        if (
            chargeCap < Settings::MIN_CAP ||
            chargeCap >
                Settings::MAX_ENCHANTING_CHARGE_CAP)
        {
            return false;
        }

        if (linearCharge > 1)
        {
            return false;
        }

        // -------------------------------------------------------------
        // Skill XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            auto &skillExp =
                skillExpGain[skillSlot];

            if (
                !ReadValue(
                    serialization,
                    skillExp.baseHundredths,
                    bytesReadTotal,
                    length) ||
                !ReadValue(
                    serialization,
                    skillExp.offsetHundredths,
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                skillExp.baseHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS ||
                skillExp.offsetHundredths >
                    Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.bySkillLevel))
            {
                return false;
            }

            if (
                !ReadSkillExpBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    skillExp.byCharacterLevel))
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // Player Level XP
        // -------------------------------------------------------------

        for (
            std::size_t skillSlot = 0;
            skillSlot < Settings::SKILL_COUNT;
            ++skillSlot)
        {
            auto &levelExp =
                playerLevelExp[skillSlot];

            if (
                !ReadValue(
                    serialization,
                    levelExp.multiplierHundredths,
                    bytesReadTotal,
                    length))
            {
                return false;
            }

            if (
                levelExp.multiplierHundredths >
                Settings::MAX_MULTIPLIER_HUNDREDTHS)
            {
                return false;
            }

            if (
                !ReadMultiplierBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    levelExp.bySkillLevel))
            {
                return false;
            }

            if (
                !ReadMultiplierBreakpointTable(
                    serialization,
                    bytesReadTotal,
                    length,
                    levelExp.byCharacterLevel))
            {
                return false;
            }
        }

        // -------------------------------------------------------------
        // PerksAtLevelUp
        // -------------------------------------------------------------

        if (
            !ReadPerksAtLevelUpBreakpointTable(
                serialization,
                bytesReadTotal,
                length,
                perksAtLevelUp))
        {
            return false;
        }

        // -------------------------------------------------------------
        // Exact record-size validation
        // -------------------------------------------------------------

        if (
            bytesReadTotal != length)
        {
            SKSE::log::error(
                "Unexpected UncapperMCM V5 "
                "record size. Read {}, expected {}.",
                bytesReadTotal,
                length);

            return false;
        }

        // -------------------------------------------------------------
        // Commit only once everything has been validated.
        // -------------------------------------------------------------

        settings.enabled =
            enabled != 0;

        settings.skillCaps =
            skillCaps;

        settings.formulaCaps =
            formulaCaps;

        settings.enchanting.magnitudeLevelCap =
            magnitudeCap;

        settings.enchanting.chargeLevelCap =
            chargeCap;

        settings.enchanting.useLinearChargeFormula =
            linearCharge != 0;

        settings.skillExpGain =
            std::move(skillExpGain);

        settings.playerLevelExp =
            std::move(playerLevelExp);

        settings.perksAtLevelUp =
            std::move(perksAtLevelUp);

        return true;
    }

    // -------------------------------------------------------------------------
    // Load callback
    // -------------------------------------------------------------------------

    void LoadCallback(
        SKSE::SerializationInterface *serialization)
    {
        // Always establish the current real INI baseline first.
        //
        // This lets older save versions inherit settings that did not
        // exist when those saves were created.
        Settings::Reset();

        std::uint32_t type = 0;
        std::uint32_t version = 0;
        std::uint32_t length = 0;

        while (
            serialization->GetNextRecordInfo(
                type,
                version,
                length))
        {
            if (
                type != RECORD_SETTINGS)
            {
                continue;
            }

            bool loaded = false;

            if (
                version ==
                RECORD_VERSION_V1)
            {
                loaded =
                    LoadVersion1(
                        serialization,
                        length);
            }
            else if (
                version ==
                RECORD_VERSION_V2)
            {
                loaded =
                    LoadVersion2(
                        serialization,
                        length);
            }
            else if (
                version ==
                RECORD_VERSION_V3)
            {
                loaded =
                    LoadVersion3(
                        serialization,
                        length);
            }
            else if (
                version ==
                RECORD_VERSION_V4)
            {
                loaded =
                    LoadVersion4(
                        serialization,
                        length);
            }
            else if (
                version ==
                RECORD_VERSION_V5)
            {
                loaded =
                    LoadVersion5(
                        serialization,
                        length);
            }
            else
            {
                SKSE::log::warn(
                    "Unsupported UncapperMCM "
                    "save record version {}.",
                    version);

                continue;
            }

            if (!loaded)
            {
                SKSE::log::error(
                    "Failed to load UncapperMCM "
                    "serialization record version {}.",
                    version);

                return;
            }

            if (
                !Settings::Apply())
            {
                SKSE::log::error(
                    "Failed to apply loaded "
                    "UncapperMCM settings.");

                return;
            }

            SKSE::log::info(
                "UncapperMCM settings loaded and applied "
                "(serialization version {}).",
                version);

            return;
        }

        SKSE::log::info(
            "No UncapperMCM save data found. "
            "Using SkyrimUncapper.ini defaults.");
    }

    // -------------------------------------------------------------------------
    // Revert
    // -------------------------------------------------------------------------

    void RevertCallback(
        SKSE::SerializationInterface *)
    {
        Settings::Reset();

        SKSE::log::info(
            "UncapperMCM settings reverted.");
    }
}

namespace Serialization
{
    void Register()
    {
        auto serialization =
            SKSE::GetSerializationInterface();

        if (!serialization)
        {
            SKSE::stl::report_and_fail(
                "SKSE serialization interface unavailable.");
        }

        serialization->SetUniqueID(
            SERIALIZATION_ID);

        serialization->SetSaveCallback(
            SaveCallback);

        serialization->SetLoadCallback(
            LoadCallback);

        serialization->SetRevertCallback(
            RevertCallback);

        SKSE::log::info(
            "SKSE serialization registered.");
    }
}
