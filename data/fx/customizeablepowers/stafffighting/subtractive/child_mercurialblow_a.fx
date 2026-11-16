#
FxInfo

	LifeSpan 10


###########################################################

	Condition
		On Time
		Time 7

		Event
			At WepL
			Type Local
			BhvrOverride
				PositionOffset 0 -0.5 -2
				PyrRotate 0 80 0
			End
			Part CustomizeablePowers\StaffFighting\MercurialBlow_StaffImage.part
			LifeSpan 2
		End

		Event
			At WepL
			Type Local
			BhvrOverride
				PositionOffset 0 -0.5 -2
				PyrRotate 0 -150 0
			End
			Part CustomizeablePowers\StaffFighting\MercurialBlow_Swish.part
			Part CustomizeablePowers\StaffFighting\MercurialBlow_Blur.part

			LifeSpan 2
		End
	End

End
