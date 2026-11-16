#############################################################
## HEADER
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
		Part1	:YellowUpGlow.part
		Part2	:YellowUpSpecks2.part
		Sound aim2 60 60 .6
	End

End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Part2	:YellowUpRing01.part
		Part3	:YellowUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 3
	End

End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Part3	:YellowUpRing02.part
		Part4	:YellowUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Part2	:YellowUpRingSlow.part
		Part3	:YellowUpRing03.part
		Part4	:YellowUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End
End

#############################################################

End