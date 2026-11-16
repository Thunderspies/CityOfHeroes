#
FxInfo

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	EfficientAdaptation_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName DrawIn
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Streaks.part
			Part CustomizeablePowers\BioArmor\Defense_Glow.part
			Part CustomizeablePowers\BioArmor\Defense_DrawIn_Spray.part
			LifeSpan 30
		End

		Event
			At Chest
			Type local
			ChildFX :Adapt_Efficient_Child.fx
		End
	End



End
