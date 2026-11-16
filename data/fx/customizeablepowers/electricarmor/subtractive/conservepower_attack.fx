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
		Type	Local
		At	Chest
		Sound elecbuildup2b 60 60 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityArchLarge2A.part
		Part	:ElectricityArchLargeA.part
		Lifespan 13
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\BuildUpGlow.part
		Part	Powers\BuildUpSpecks2.part
	End
End

Condition
	On	Time
	Time	10

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\BuildUpRing01.part
		Part	Powers\BuildUpRing01a.part
	End
End

Condition
	On	Time
	Time	20

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\BuildUpRing02.part
		Part	Powers\BuildUpRing02a.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	Powers\BuildUpRingSlow.part
		Part	Powers\BuildUpRing03.part
		Part	Powers\BuildUpRing03a.part
	End
End

#############################################################

End