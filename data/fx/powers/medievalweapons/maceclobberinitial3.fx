#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Lifespan 60

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	58

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		LifeSpan	1
	End

End

	
End	

	