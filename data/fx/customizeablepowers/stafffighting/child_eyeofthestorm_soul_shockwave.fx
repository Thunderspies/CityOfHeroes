#
FxInfo

	LifeSpan 15


###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHESOUL
		Time 20


		Event
			At Root
			Type Start
			Part CustomizeablePowers\StaffFighting\Shockwave_Soul.part
			LifeSpan 14
		End

	End

End
