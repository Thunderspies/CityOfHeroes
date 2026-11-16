#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	20

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\FollowUpGlow.part
		Part2	POWERS\FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Lifespan 13
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part1	POWERS\FollowUpSpecks.part
		Part2	POWERS\FollowUpRing01.part
		Part3	POWERS\FollowUpRing01a.part
	End

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part1	POWERS\FollowUpSpecks.part
		Part3	POWERS\FollowUpRing02.part
		Part4	POWERS\FollowUpRing02a.part
	End
End

Condition
	On	Time
	Time	50

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		#Part1	POWERS\FollowUpSpecks2.part
		Part2	POWERS\FollowUpRingSlow.part
		Part3	POWERS\FollowUpRing03.part
		Part4	POWERS\FollowUpRing03a.part
	End
End

#############################################################

End