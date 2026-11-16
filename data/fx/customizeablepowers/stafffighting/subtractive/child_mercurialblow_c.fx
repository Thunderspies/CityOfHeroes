#
FxInfo

	LifeSpan 10


###########################################################

	Condition
		On Time
		Time 5

		Event
			At WepL
			Type Local
			BhvrOverride
				PositionOffset 0 1 0
				PyrRotate 0 150 0
			End
			Part CustomizeablePowers\StaffFighting\Subtractive\MercurialBlow_StaffImage.part
			Part CustomizeablePowers\StaffFighting\Subtractive\MercurialBlow_Swish.part
			Part CustomizeablePowers\StaffFighting\Subtractive\MercurialBlow_Blur.part

			LifeSpan 5
		End
	End

End
