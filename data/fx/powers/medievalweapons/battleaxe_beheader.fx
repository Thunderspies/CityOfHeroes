#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 17

#############################################################

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
		bhvr	behaviors\Medieval1.bhvr
		Geom	swoosh
	End
End

#############################################################

End