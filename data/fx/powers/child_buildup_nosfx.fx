#############################################################
## Child_BuildUp.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\FollowUpGlow.part
		Part2	Powers\FollowUpSpecks2.part
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\FollowUpRing01.part
		Part3	Powers\FollowUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	Powers\FollowUpRing02.part
		Part4	Powers\FollowUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\FollowUpRingSlow.part
		Part3	Powers\FollowUpRing03.part
		Part4	Powers\FollowUpRing03a.part
	End
End

#############################################################

End