#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Lifespan 55


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
	Time 	54

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		LifeSpan	1
	End

End

	
End	

	