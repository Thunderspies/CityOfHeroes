#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

Lifespan 1

Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/fadeout.bhvr
		Flags	AdoptParentEntity
	End
End


End						