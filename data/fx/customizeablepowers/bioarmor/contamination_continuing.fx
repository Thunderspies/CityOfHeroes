#                                                          
FxInfo


###########################################################

	Condition
		On Cycle
		Time 10

		Event
			At Chest
			Type Local
			EName RINGS
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part CustomizeablePowers\BioArmor\Contamination_Glow.part
			Part CustomizeablePowers\BioArmor\Power_Ring_Continuing.part
			LifeSpan 20
		End

	End

End
