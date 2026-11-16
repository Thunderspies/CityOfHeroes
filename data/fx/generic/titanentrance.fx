#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 200

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	Origin 
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
		Lifespan 30
	End
End




End			