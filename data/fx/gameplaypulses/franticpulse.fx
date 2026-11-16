#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On Cycle
	Time 30
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	behaviors/FranticlightPulse.bhvr
		LifeSpan 20
End


End			