#############################################################
## PowerBoost_Attack.fx
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
		Part	Powers\ChestLevelUpRaysThinFaint.part
		Part1	Powers\MultiUpGlow.part
		Part2	Powers\MultiUpSpecks2.part
		Sound aim4 60 60 .6
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
		Part2	Powers\MultiUpRing01.part
		Part3	Powers\MultiUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	Powers\MultiUpRing02.part
		Part4	Powers\MultiUpRing02a.part
	End

End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\MultiUpRingSlow.part
		Part3	Powers\MultiUpRing03.part
		Part4	Powers\MultiUpRing03a.part
	End
End

#############################################################

End