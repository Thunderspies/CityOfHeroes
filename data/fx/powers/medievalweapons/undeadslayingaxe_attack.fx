#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	17

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
		LifeSpan 25
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	Mystic
		bhvr	behaviors\Medieval1.bhvr
		Geom	swoosh
		Lifespan 1
	End
End

#############################################################

End