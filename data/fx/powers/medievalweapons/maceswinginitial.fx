#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Lifespan 49

Condition
	On 	Time
	Time 	12

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	48

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
	End

End

	
End	

	