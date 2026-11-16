#############################################################
## ChockingSporesHit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	Head
		Part	CustomizeablePowers\PlantControl\ChockingSporesHitDots.part
		Part	CustomizeablePowers\PlantControl\ChockingSporesHit.part
		Part	CustomizeablePowers\PlantControl\ChockingSporesHit2.part
		PMagnet	Head
		Sound mushroomhit3_loop 40 40 0.3
	End
End

#############################################################

End