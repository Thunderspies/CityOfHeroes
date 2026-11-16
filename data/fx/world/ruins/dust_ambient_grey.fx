#                                                          
FxInfo

	Input
		InpName Origin
	End

	LifeSpan 0


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part :Dust_Ambient_Grey01.part
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0.0 6.0 0.0
			End
			Part :Dust_Ambient_Grey02.part
		End

	End

End
