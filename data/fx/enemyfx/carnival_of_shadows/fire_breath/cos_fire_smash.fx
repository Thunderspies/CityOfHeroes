#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 26


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
	Time 	23

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
	End

End

	
End	

	