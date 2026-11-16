#
FxInfo

Lifespan 2

###########################################################

	Condition
		ON		TRIGGERBITS
		TRIGGERBITS	STAFFMASTERY FORMOFTHESOUL

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 1 7
				PyrRotate 20 15 90
				Spin 0 0.5 0
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Soul_Large.part
			LifeSpan 2
		End

	End

###########################################################

End
