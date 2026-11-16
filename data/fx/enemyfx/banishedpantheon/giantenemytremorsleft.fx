#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	ToeL
End

LifeSpan	10

Condition
	
	Event
			
		Type	start
		At	ToeL
		Bhvr	Behaviors/EarthQuakeSteps.bhvr
		Part	:GiantFootDust.part
		part	:GiantFootDust2.part
		part	:GiantFootStepRocks.part
		
	End

End



End			