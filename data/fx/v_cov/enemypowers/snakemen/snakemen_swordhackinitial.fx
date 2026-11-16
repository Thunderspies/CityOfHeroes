#########################################################
##	template

FxInfo


Lifespan 55

Flags	InheritAnimScale

Condition
	On 	Time
	Time 	48

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	52

	Event
		EName	Streak
		Type	Local
		At	Origin
		
		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrailVert.bhvr
		LifeSpan 3
	End

End

	
End	

	