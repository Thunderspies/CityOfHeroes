#                                                          
FxInfo

	Input
		InpName Head
	End

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			Part :Agility.part
			Part :AgilitySmall.part
			Part :AbilityDots.part
			Part :AbilitySparklingDots.part
			Sound mabuildup2 70 70 0.7
		End

	End

End
