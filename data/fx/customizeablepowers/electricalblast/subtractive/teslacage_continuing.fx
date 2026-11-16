#########################################################
##	forceBubble
########################################################
FxInfo

########################################################





Condition
	On 	Time
	Time 	0

	Event

		Type	PositOnly
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityCageRingBright.part
		Sound eleccage_loop 100 100 .8

	End

End

Condition
	On 	Time
	Time 	15

	Event

		Type	PositOnly
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityCageArchs2.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityCageGlow.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityCageArchs.part

	End


	Event

		Type	PositOnly
		At	Hips
		Geom	Cagearchs2
		bhvr	CustomizeablePowers\ElectricalBlast\cagespin2.bhvr
	End



End

Condition
	On 	Time
	Time 	16
	Event

		Type	PositOnly
		At	Hips
		Geom	Cagearchs
		bhvr	CustomizeablePowers\ElectricalBlast\cagespin.bhvr
	End


End

End
