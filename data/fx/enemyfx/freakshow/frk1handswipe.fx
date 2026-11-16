#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 30

#############################################################

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
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	:FRKSmashFadeOut01.bhvr
		Lifespan 5
	End
End

#############################################################

End