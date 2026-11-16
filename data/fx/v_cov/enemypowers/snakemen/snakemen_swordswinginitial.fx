#########################################################
##	template

FxInfo


Lifespan 52

Flags	InheritAnimScale

Condition
	On 	Time
	Time 	43

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .75
	End

End

Condition
	On 	Time
	Time 	47

	Event
		EName	Streak
		Type	Local
		At	Origin
		
		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrail.bhvr
		LifeSpan	4
	End

End

	
End	

	