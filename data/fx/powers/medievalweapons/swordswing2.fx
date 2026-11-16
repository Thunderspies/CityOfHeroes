#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 38


Condition
	On 	Time
	Time 	13

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .75
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		LifeSpan	1
	End

End

	
End	

	