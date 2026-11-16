#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type PositOnly
			Part :BigLeafPlant.part
			Part :BigLeafPlant2.part
			Part AurasPath\Shrubbery\BigLeafPlant3.part
			Part :GroundShrub2.part
			Part :GroundShrub3.part
			Part :IdleShrub.part
		End

	End

	Condition
		On Cycle
		Time 10
		Chance 0.5

		Event
			At Root
			Type StartPositOnly
			Part AurasPath\Shrubbery\LeafBurst.part
			Part AurasPath\Shrubbery\LeafBurst2.part
			LifeSpan 3
		End

	End

End
