#########################################################
##	template

FxInfo
Lifespan 75

#########################################################

Condition
	On time
	Time 0

	Event

		Type	Local
		At	Chest
		Sound aim3 60 60 .6
		End
end



Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	CustomizeablePowers\EnergyAura\BuildUpGlow.part
		Part2	CustomizeablePowers\EnergyAura\BuildUpSpecks2.part
		#Sound	clawBuild3 100 30 .8
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 13
	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks.part
		Part2	CustomizeablePowers\EnergyAura\BuildUpRing01.part
		Part3	CustomizeablePowers\EnergyAura\BuildUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks.part
		Part3	CustomizeablePowers\EnergyAura\BuildUpRing02.part
		Part4	CustomizeablePowers\EnergyAura\BuildUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:BuildUpSpecks2.part
		Part2	CustomizeablePowers\EnergyAura\BuildUpRingSlow.part
		Part3	CustomizeablePowers\EnergyAura\BuildUpRing03.part
		Part4	CustomizeablePowers\EnergyAura\BuildUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr

	End

End