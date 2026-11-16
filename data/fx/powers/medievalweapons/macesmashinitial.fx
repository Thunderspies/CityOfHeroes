#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Lifespan 47

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	46

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
	End

End

	
End	

	