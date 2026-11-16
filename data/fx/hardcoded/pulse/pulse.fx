#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On Cycle
	Time 120
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	behaviors/lightPulse.bhvr
		LifeSpan 90
End


End			