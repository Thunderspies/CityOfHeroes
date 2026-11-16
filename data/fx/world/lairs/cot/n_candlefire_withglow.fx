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
				PositionOffset 1 3 5
			End
			Part WORLD\Lairs\cot\N_Candleflame.part
			Part WORLD\Lairs\cot\N_Candleflame_Core.part
			Part WORLD\Lairs\cot\N_Candleflame_Glow_Core.part
			Part WORLD\Lairs\cot\N_Candleflame_Glow.part
		End

	End

End
