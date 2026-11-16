#########################################################
##	template

FxInfo

Condition
	On 	Time
	Time 	17

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	22

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan 1
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	all
		Type	Destroy
		
	End

End

End

	