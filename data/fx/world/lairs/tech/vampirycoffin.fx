#########################################################
##	tech_rings1
##
FxInfo

Lifespan 210	


Condition
	On 	Time
	Time 	0

	Event

		Type	Local 
		At	Origin
		BHVR	:ChamberOffset.bhvr
		Part	:ColdSmokeDown.part
		LifeSpan 200
	End

	Event

		Type	Local 
		At	Origin
		BHVR	:ChamberOffsetRight.bhvr
		Part	:ColdSmokeDown.part
		LifeSpan 190
	End

	Event

		Type	Local 
		At	Origin
		BHVR	:ChamberOffsetLeft.bhvr
		Part	:ColdSmokeDown.part
		LifeSpan 190
		
	End

End

Condition
	On 	Time
	Time 	91

	Event

		Type	Local 
		At	Origin
		BHVR	:ChamberLightRaysOffset.bhvr
		Part	:RedLightRays.part
		Part	:RedLightRaysCore.part
		Part	:RedLightCore.part
		LifeSpan 3
	End

End

Condition
	On 	Time
	Time 	63

	Event

		Type	Local 
		At	root
		
		Part	:ColdSmokeBase.part
		Part	:ColdSmokeBaseFlat.part
		LifeSpan 200
		
	End

End

End			