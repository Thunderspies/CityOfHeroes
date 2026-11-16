#
FxInfo

	Input
		InpName Chest
	End


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\MartialManipulation\Sand_Dust.part
			LifeSpan 2
		End

	End

End
