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
		Bhvr	:lightPulse.bhvr
		LifeSpan 90
	End
End

Condition
	On	Cycle
	Time	120
	
	Event	
		Type	Local
		Sound ImDestructible 80.0 80.0 .5
		Lifespan 1
	End

End		