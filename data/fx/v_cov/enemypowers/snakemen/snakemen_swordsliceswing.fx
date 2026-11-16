#########################################################
##	template

FxInfo

Flags	InheritAnimScale


LifeSpan	75

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time	28
	

	Event
		EName	Streak
		Type	Local		
		At	Origin
		
		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrailReverse.bhvr
		Lifespan 4
	End

End

	
End	

	