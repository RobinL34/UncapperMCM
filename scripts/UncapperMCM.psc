ScriptName UncapperMCM

Bool Function GetEnabled() Global Native
Function SetEnabled(Bool enabled) Global Native

Int Function GetSkillCap(Int skillSlot) Global Native
Bool Function SetSkillCap(Int skillSlot, Int value) Global Native

Int Function GetFormulaCap(Int skillSlot) Global Native
Bool Function SetFormulaCap(Int skillSlot, Int value) Global Native

Int Function GetEnchantMagnitudeCap() Global Native
Bool Function SetEnchantMagnitudeCap(Int value) Global Native

Int Function GetEnchantChargeCap() Global Native
Bool Function SetEnchantChargeCap(Int value) Global Native

Bool Function GetEnchantUseLinearCharge() Global Native
Function SetEnchantUseLinearCharge(Bool enabled) Global Native


; ---------------------------------------------------------------------------
; Skill XP
; ---------------------------------------------------------------------------

Int Function GetSkillExpBaseMultiplier(Int skillSlot) Global Native
Int Function GetSkillExpOffsetMultiplier(Int skillSlot) Global Native

Bool Function SetSkillExpBaseMultipliers(Int skillSlot, Int baseHundredths, Int offsetHundredths) Global Native

Int Function GetSkillExpBreakpointCount(Int skillSlot, Bool characterLevel) Global Native
Int Function GetSkillExpBreakpointLevel(Int skillSlot, Bool characterLevel, Int index) Global Native
Int Function GetSkillExpBreakpointBaseMultiplier(Int skillSlot, Bool characterLevel, Int index) Global Native
Int Function GetSkillExpBreakpointOffsetMultiplier(Int skillSlot, Bool characterLevel, Int index) Global Native

Bool Function SetSkillExpBreakpoint(Int skillSlot, Bool characterLevel, Int index, Int level, Int baseHundredths, Int offsetHundredths) Global Native
Bool Function AddSkillExpBreakpoint(Int skillSlot, Bool characterLevel, Int level, Int baseHundredths, Int offsetHundredths) Global Native
Bool Function RemoveSkillExpBreakpoint(Int skillSlot, Bool characterLevel, Int index) Global Native


; ---------------------------------------------------------------------------
; Player Level XP
; ---------------------------------------------------------------------------

Int Function GetPlayerLevelExpMultiplier(Int skillSlot) Global Native
Bool Function SetPlayerLevelExpMultiplier(Int skillSlot, Int multiplierHundredths) Global Native

Int Function GetPlayerLevelExpBreakpointCount(Int skillSlot, Bool characterLevel) Global Native
Int Function GetPlayerLevelExpBreakpointLevel(Int skillSlot, Bool characterLevel, Int index) Global Native
Int Function GetPlayerLevelExpBreakpointMultiplier(Int skillSlot, Bool characterLevel, Int index) Global Native

Bool Function SetPlayerLevelExpBreakpoint(Int skillSlot, Bool characterLevel, Int index, Int level, Int multiplierHundredths) Global Native
Bool Function AddPlayerLevelExpBreakpoint(Int skillSlot, Bool characterLevel, Int level, Int multiplierHundredths) Global Native
Bool Function RemovePlayerLevelExpBreakpoint(Int skillSlot, Bool characterLevel, Int index) Global Native


; ---------------------------------------------------------------------------
; Perks at Level Up
; ---------------------------------------------------------------------------

Int Function GetPerksAtLevelUpBreakpointCount() Global Native
Int Function GetPerksAtLevelUpBreakpointLevel(Int index) Global Native
Int Function GetPerksAtLevelUpBreakpointValue(Int index) Global Native

Bool Function SetPerksAtLevelUpBreakpoint(Int index, Int level, Int valueHundredths) Global Native
Bool Function AddPerksAtLevelUpBreakpoint(Int level, Int valueHundredths) Global Native
Bool Function RemovePerksAtLevelUpBreakpoint(Int index) Global Native


; ---------------------------------------------------------------------------
; Attributes at Level Up
; ---------------------------------------------------------------------------

Bool Function GetIniUseAttributesAtLevelUp() Global Native

Int Function GetAttributeBreakpointCount(Int tableIndex) Global Native
Int Function GetAttributeBreakpointLevel(Int tableIndex, Int index) Global Native
Int Function GetAttributeBreakpointValue(Int tableIndex, Int index) Global Native

Bool Function SetAttributeBreakpoint(Int tableIndex, Int index, Int level, Int value) Global Native
Bool Function AddAttributeBreakpoint(Int tableIndex, Int level, Int value) Global Native
Bool Function RemoveAttributeBreakpoint(Int tableIndex, Int index) Global Native


; ---------------------------------------------------------------------------
; General
; ---------------------------------------------------------------------------

Bool Function ClearOverrides() Global Native