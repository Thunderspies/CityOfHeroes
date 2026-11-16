#
FxInfo

	LifeSpan 60


###########################################################

	Condition
		On		Time
		Time		35

		Event
			EName	SoulWaves
			At WepL
			Type Local
			BhvrOverride
				PyrRotate 0 0 90
			End
			Part CustomizeablePowers\StaffFighting\Center_Swish_Mind.part
			LifeSpan 5
		End

	End

	Condition
		On		Time
		Time		45

		Event
			At Root
			Type Start
			Part CustomizeablePowers\StaffFighting\Shockwave_Mind.part
			LifeSpan 14
		End

	End

End
