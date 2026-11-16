#
FxInfo

	LifeSpan 5


###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHESOUL

		Event
			EName	SoulWaves
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Soul.part
			LifeSpan 5
		End

	End

End
