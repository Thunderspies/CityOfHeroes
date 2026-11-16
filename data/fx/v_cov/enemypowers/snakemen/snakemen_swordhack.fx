#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

LifeSpan	25

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	22

	Event
		EName	Streak
		Type	Local
		At	Origin
		
		Geom	swoosh
		bhvr	behaviors/FX_SnakeMen_SwordTrailVert.bhvr
		Lifespan 3
	End

End

	
End	

	