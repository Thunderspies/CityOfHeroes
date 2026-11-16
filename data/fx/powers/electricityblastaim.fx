#############################################################
## ElectricityBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	chest
		Childfx	Powers\ElectricityControl\buildupChragedBrawlHit.fx
		LifeSpan	70
		LifeSpanJitter	40
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Chest
		Bhvr	Powers\GenericParticleFade.bhvr
		Part1	Powers\YellowUpGlow.part
		Part2	Powers\YellowUpSpecks2.part
		Sound aim2 60 60 .6
		Lifespan 13
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Powers\GenericParticleFade.bhvr
		Part2	Powers\YellowUpRing01.part
		Part3	Powers\YellowUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Powers\GenericParticleFade.bhvr
		Part3	Powers\YellowUpRing02.part
		Part4	Powers\YellowUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Powers\GenericParticleFade.bhvr
		Part2	Powers\YellowUpRingSlow.part
		Part3	Powers\YellowUpRing03.part
		Part4	Powers\YellowUpRing03a.part
	End
End

#############################################################

End