#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 20


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .75
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/sword_streak.bhvr
		Lifespan 2
	End

End

	
End	

	