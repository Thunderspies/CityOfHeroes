#############################################################
## PoisonAOEGassesExplode.fx
#############################################################

FxInfo

#############################################################

Condition

	Event
		EName	minepivot
		Type	start
		At	root
		Geom	gadgetHookup
	End

	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Part	CustomizeablePowers\Poison\AOETrailBlack.part
		Part	CustomizeablePowers\Poison\AOETrailLight.part
		Part	CustomizeablePowers\Poison\AOETrailStreak.part
		Part	CustomizeablePowers\Poison\AOEYellowSmallBallzTrail.part
		Part	CustomizeablePowers\Poison\AOEBallzTrail.part
		Part	CustomizeablePowers\Poison\AOEBallzTrailBlack.part
	End
End

#############################################################

End