#############################################################
## EnergyMelee_FollowUpHit.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	start
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\EnergyMelee\PowerPunchHit.part
		Part2	CustomizeablePowers\EnergyMelee\PowerPunchHit1.part
		Part3	CustomizeablePowers\EnergyMelee\PowerPunchHit2.part
		Part4	CustomizeablePowers\EnergyMelee\PowerPunchHitRaysSmall.part
		Part1	CustomizeablePowers\EnergyMelee\HitRing.part
		Part3	CustomizeablePowers\EnergyMelee\HitRing2.part
		Part2	CustomizeablePowers\EnergyMelee\PowerPunchBallz.part
		Part4	CustomizeablePowers\EnergyMelee\PowerPunchRingz.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End