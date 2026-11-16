#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End


Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/quick_fadein.bhvr
		Flags	AdoptParentEntity
	End
End


End			