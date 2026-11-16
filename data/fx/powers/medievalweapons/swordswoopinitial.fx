#########################################################
##	template

FxInfo

LifeSpan	52

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End

Condition
	On 	Time
	Time 	51

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		Lifespan 1
	End

End


	
End	

	