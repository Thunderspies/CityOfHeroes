#############################################################
## ConservePower_Attack.fx
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
		Part1	Powers\BuildUpGlow.part
		Part2	Powers\BuildUpSpecks2.part
		Sound aim3 60 60 .6
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
		Part2	Powers\BuildUpRing01.part
		Part3	Powers\BuildUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	Powers\BuildUpRing02.part
		Part4	Powers\BuildUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\BuildUpRingSlow.part
		Part3	Powers\BuildUpRing03.part
		Part4	Powers\BuildUpRing03a.part
	End
End

#############################################################

End