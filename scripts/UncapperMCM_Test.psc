ScriptName UncapperMCM_Test extends Quest

Event OnInit()

    Debug.Trace("[UncapperMCM Test] Starting runtime API test...")

    Bool skillResult = UncapperMCM.SetSkillCap(0, 160)
    Bool formulaResult = UncapperMCM.SetFormulaCap(0, 160)

    If skillResult && formulaResult
        Debug.Trace("[UncapperMCM Test] One-Handed caps successfully set to 160.")
        Debug.Notification("UncapperMCM test: One-Handed cap = 160")
    Else
        Debug.Trace("[UncapperMCM Test] ERROR while setting One-Handed caps.")
        Debug.Notification("UncapperMCM test failed")
    EndIf

EndEvent