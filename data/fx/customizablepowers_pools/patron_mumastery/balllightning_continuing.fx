#                                                          
FxInfo

	Input
		InpName Hips
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Flags PowerLoopingSound
			Bhvr behaviors\soundFade3.bhvr
			Sound eleccage_loop 70 70 0.2
			LifeSpan 390
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\ElectricalBlast\ElectricityArch4a.part
			Part CustomizeablePowers\ElectricalBlast\ElectricityArchSmalla.part
			Part CustomizeablePowers\ElectricalBlast\ElectricityGlow.part
			Part CustomizeablePowers\ElectricalBlast\ElectricityGlowStar.part
			Sound elecballhit2 50 50 0.33
			LifeSpan 21
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing2.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing2.part
		End

	End

	Condition
		On Time
		Time 3

		Event
			At Chest
			Type Local
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing2.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing2.part
			Part CustomizeablePowers\ContinuingConditionalFX\ElectricityArchContinuing.part
		End

	End

End
