####################
## Swimming Shark ##	
####################

FxInfo

Input  
	InpName	Origin
End

##Lifespan 1

Condition
	On Time
	Time 0
	Event
		EName 	Shark_1
		Type	Local
		At	Hips
		Anim	Shark_Encounter
		
		BhvrOverride
#			Startjitter	.5 3 .5
			PositionOffset	0 -0.125 0
			Scale		0.75 0.75 0.75
	
	End

End

End						