#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End

Flags    InheritAlpha

Lifespan 60

Condition
	On 	Time
	Time 	12

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	:FRKSmashFadeOut01.bhvr
		Lifespan 5
	End

End

	
End	

	