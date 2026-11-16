#########################################################
##	template

FxInfo

Flags	InheritAnimScale

LifeSpan	75

#Condition
#	On	Time
#	Time	52
#
#	Event	
#		Type	Local
#		At	Origin
#		Sound	Whooshbig3 100.0 30.0 .8
#
#	End
#End

Condition
	On 	Time
	Time 	55

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	58

	Event
		EName	Streak
		Type	Local
		At	Origin
		
		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrailReverse.bhvr
		Sound Whoosh5 100.0 100.0 .6
		Lifespan 4
	End

End

	
End	

	