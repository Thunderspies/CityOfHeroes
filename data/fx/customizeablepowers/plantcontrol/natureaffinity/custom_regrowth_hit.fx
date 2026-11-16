#                                                          
FxInfo

	LifeSpan 400


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName PollenAnchor
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
		End

	End

	Condition
		On Cycle
		Time 80

		Event
			At PollenAnchor
			Type PositOnly
			EName Pollen
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_pollen.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_Stars.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_Circles.part
			LifeSpan 30
		End

	End

End
