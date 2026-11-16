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
		Part	V_COV\POWERS\Poison\AOETrailBlack.part
		Part	V_COV\POWERS\Poison\AOETrailLight.part
		Part	V_COV\POWERS\Poison\AOETrailStreak.part
		Part	V_COV\POWERS\Poison\AOEYellowSmallBallzTrail.part
		Part	V_COV\POWERS\Poison\AOEBallzTrail.part
		Part	V_COV\POWERS\Poison\AOEBallzTrailBlack.part
	End
End

#############################################################

End