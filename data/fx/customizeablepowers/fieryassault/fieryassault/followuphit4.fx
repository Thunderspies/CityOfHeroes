#########################################################
##	template

FxInfo
Lifespan 75
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
		Part1	CustomizeablePowers\FieryAssault\FollowUpGlow.part
		Part2	CustomizeablePowers\FieryAssault\FollowUpSpecks2.part
		#Sound	clawfollow3 100 30 .8
		Bhvr	behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part2	CustomizeablePowers\FieryAssault\FollowUpRing01.part
		Part3	CustomizeablePowers\FieryAssault\FollowUpRing01a.part
		Bhvr	behaviors\GenericParticleFade.bhvr
	End


	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part3	CustomizeablePowers\FieryAssault\FollowUpRing02.part
		Part4	CustomizeablePowers\FieryAssault\FollowUpRing02a.part
		Bhvr	behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 50

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks2.part
		Part2	CustomizeablePowers\FieryAssault\FollowUpRingSlow.part
		Part3	CustomizeablePowers\FieryAssault\FollowUpRing03.part
		Part4	CustomizeablePowers\FieryAssault\FollowUpRing03a.part
		Bhvr	behaviors\GenericParticleFade.bhvr

	End

End


Condition
	On Time
	Time 33

	Event
		EName	pow2
		Type 	Destroy

	End

End

