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
			Type Start
			EName Prime
			BhvrOverride
				PositionOffset 0 3 5
			End
			Part WORLD\Lairs\cot\N_Candleflame_Blue.part
			Part WORLD\Lairs\cot\N_Candleflame_Blue_Core.part
		End

	End

End
