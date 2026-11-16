#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 2

		Event
			At Chest
			Type Local
			Part :Flash.part
			LifeSpan 5
		End

		Event
			At Head
			Type Local
			EName faceringsRigging
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part IncarnatePowers\Interface\ThinStreaks.part
			Part IncarnatePowers\Interface\PsioticRings.part
			Part IncarnatePowers\Interface\HeadGlow_Additive.part
			Part IncarnatePowers\Interface\HeadGlow.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hair
			Type Local
			EName Hairnode
			Geom HairToHeadAdjustment
		End

		Event
			AltPiv 1
			At Hairnode
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :ConfuseDots01.part
			Part :ConfuseDots02.part
			Part :ConfuseBubbles01.part
			Part :ConfuseStars01.part
		End

	End

End
