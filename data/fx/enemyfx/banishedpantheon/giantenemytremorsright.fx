#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	ToeR
End

LifeSpan	10

Condition
	
	Event
			
		Type	start
		At	ToeR
		Bhvr	Behaviors/EarthQuakeSteps.bhvr
		Part	:GiantFootDust.part
		part	:GiantFootDust2.part
		part	:GiantFootStepRocks.part
		
	End



End



End			