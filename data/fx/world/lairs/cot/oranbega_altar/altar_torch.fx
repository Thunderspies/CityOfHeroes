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
			Type StartPositOnly
			EName torch
			BhvrOverride
				PositionOffset 0 12 0
			End
		End

		Event
			At torch
			Type Local
			EName Prime
			BhvrOverride
				PositionOffset 0 -1.2 0
			End
			Part WORLD\Lairs\cot\Oranbega_Altar\AltarFlame_BaseGlow_BottomFlat.part
			Part WORLD\Lairs\cot\Oranbega_Altar\AltarFlame_BaseGlow.part
		End

		Event
			At torch
			Type Local
			EName Prime
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :Altar_Torch02.part
		End

	End

End
