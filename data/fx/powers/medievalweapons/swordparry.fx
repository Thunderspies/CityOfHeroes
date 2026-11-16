#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 60


Condition
	On 	Time
	Time 	8

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .5
	End

End





Condition
	On 	Time
	Time 	10

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		LifeSpan 1
	End

End


Condition
	On 	Time
	Time 	19

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	22

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		LifeSpan 1
	End

End

End

	