#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Lifespan 17

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	16

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		
	End

End

	
End	

	