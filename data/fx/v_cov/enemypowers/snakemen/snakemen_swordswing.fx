#########################################################
##	template

FxInfo

Flags	InheritAnimScale

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 23


Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Origin
		Sound Whoosh5 100.0 100.0 .75
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName	Streak
		Type	Local
		At	Origin

		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrail.bhvr
		lifespan 4
	End

End

	
End	

	