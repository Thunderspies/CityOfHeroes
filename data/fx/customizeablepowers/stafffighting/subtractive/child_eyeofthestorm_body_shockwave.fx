#
FxInfo

	LifeSpan 15


###########################################################

	Condition
		On		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHEBODY
		Time 20


		Event
			At Root
			Type Start
			Part CustomizeablePowers\StaffFighting\Shockwave_Body.part
			Part CustomizeablePowers\StaffFighting\Shockwave_Body_Burst.part
			LifeSpan 14
		End

	End

End
