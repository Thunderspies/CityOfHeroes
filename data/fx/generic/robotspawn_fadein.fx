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
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/RobotSpawn_fadein.bhvr
		Flags	AdoptParentEntity
	End
End


End			