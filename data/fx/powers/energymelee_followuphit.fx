#############################################################
## EnergyMelee_FollowUpHit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

Lifespan 75

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	start
		At	chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\PowerPunch\PowerPunchHit.part
		Part2	Powers\PowerPunch\PowerPunchHit1.part
		Part3	Powers\PowerPunch\PowerPunchHit2.part
		Part4	Powers\PowerPunch\PowerPunchHitRaysSmall.part
		Part1	Powers\PowerPunch\HitRing.part
		Part2	Powers\PowerPunch\PowerPunchBallz.part
		Part3	Powers\PowerPunch\HitRing2.part
		Part4	Powers\PowerPunch\PowerPunchRingz.part
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