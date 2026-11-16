#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 60

Condition
	Event
		Type	Local
		At	Origin 
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
	End
End

End			