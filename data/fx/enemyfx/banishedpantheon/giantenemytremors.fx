#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	FootL
End

LifeSpan	10

Condition
	
	Event
			
		Type	start
		At	FootL
		Bhvr	Behaviors/EarthQuakeSteps.bhvr
		
		
	End

End



End			