#############################################################
## Blood Widow "Aim" FX
#############################################################

FxInfo

Lifespan 45

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	Powers\YellowUpGlow.part
		Part2	Powers\YellowUpSpecks2.part
		Sound aim2 60 60 .6
		Lifespan 3
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	Powers\YellowUpRing01.part
		Part3	Powers\YellowUpRing01a.part
		Lifespan 3
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	Powers\YellowUpRing02.part
		Part4	Powers\YellowUpRing02a.part
		Lifespan 3
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
		Part2	Powers\YellowUpRingSlow.part
		Part3	Powers\YellowUpRing03.part
		Part4	Powers\YellowUpRing03a.part
		Lifespan 3
	End
End

#############################################################

End