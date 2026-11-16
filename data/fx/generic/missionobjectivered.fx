#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Condition
	
	Event

		Type	local
		At	Origin
		Sound objective_loop 100 100 .8
	End
End



Condition
	On Cycle
	Time 30
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	behaviors/FranticlightPulseRed.bhvr
		LifeSpan 20
End


End			