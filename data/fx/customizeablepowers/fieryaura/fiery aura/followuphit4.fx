#############################################################
## FollowUpHit4.fx
#############################################################\

FxInfo
Lifespan 80

#########################################################



Condition
	On Time
	Time 0

	Event
		Type 	local
		At	Origin
		Sound firefollow 80 80 .8
		End
End


Condition
	On Time
	Time 20

	Event
		EName	Pow2
		Type 	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\FieryAura\FollowUpGlow.part
		Part2	CustomizeablePowers\FieryAura\FollowUpSpecks2.part
		#Sound	clawfollow3 100 30 .8
	End
End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks.part
		Part2	CustomizeablePowers\FieryAura\FollowUpRing01.part
		Part3	CustomizeablePowers\FieryAura\FollowUpRing01a.part
	End


	Event
		EName	Pow
		Type 	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks.part
		Part3	CustomizeablePowers\FieryAura\FollowUpRing02.part
		Part4	CustomizeablePowers\FieryAura\FollowUpRing02a.part
	End
End

Condition
	On Time
	Time 50

	Event
		EName	Pow
		Type 	local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks2.part
		Part2	CustomizeablePowers\FieryAura\FollowUpRingSlow.part
		Part3	CustomizeablePowers\FieryAura\FollowUpRing03.part
		Part4	CustomizeablePowers\FieryAura\FollowUpRing03a.part
	End
End



End

