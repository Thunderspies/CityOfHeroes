#                                                          
FxInfo

	LifeSpan 47


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound BR_PenetratingRay_Hit_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName Prime
			Part :Hit_Star.part
			Part :Hit_Flash_Small.part
			Part :Hit_Arcs.part
			LifeSpan 10
		End

	End

End
