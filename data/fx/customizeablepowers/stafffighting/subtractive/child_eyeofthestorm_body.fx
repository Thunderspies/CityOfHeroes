#
FxInfo

	LifeSpan 5


###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHEBODY

		Event
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Burst.part
			LifeSpan 5
		End

	End

End
