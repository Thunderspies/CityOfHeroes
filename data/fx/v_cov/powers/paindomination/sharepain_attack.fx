#############################################################
## AbsorbPain.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	dummy
		Type	Local
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:AbsorbPainRings.part
		Part2	:Healing02.part
		sound	Share_Pain_01 80 30 .75
		LifeSpan 45
	End
End

#############################################################

Condition
	On 	cycle
	Time 	14

	Event
		EName	dummy3
		Type	Local
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:AbsorbPainRing1.part
		LifeSpan 14
	End
End

Condition
	On 	cycle
	Time 	28

	Event
		EName	dummy4
		Type	Local
		At	chest
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:AbsorbPainRing1.part
		LifeSpan 14
	End
End

#############################################################

End