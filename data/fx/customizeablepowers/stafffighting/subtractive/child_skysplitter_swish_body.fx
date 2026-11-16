#
FxInfo

Lifespan 2

###########################################################

	Condition
		ON		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHEBODY

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 1 7
				PyrRotate 20 15 90
				Spin 0 0.5 0
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Large.part
			Part CustomizeablePowers\StaffFighting\Center_Swish_Body_Burst_Large.part
			LifeSpan 2
		End

	End

###########################################################

End
