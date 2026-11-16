#
FxInfo
Lifespan 500

###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			Part :SummonRing_LightStreaks.part
			Part :SummonRing_Poof01.part
			Part :SummonRing_LightDust.part
			Part :SummonRing_LightDust2.part
			LifeSpan 40
		End

		Event
			EName BeginAnchor
			At Root
			Type Local
			BhvrOverride
				PositionOffset -5 20 0
			End
		End

		Event
			EName EndAnchor
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0.25 2.5 0
			End
		End

		Event
			At BeginAnchor
			Type Local
			POther EndAnchor
			Part :Rainbow.part
			Part :Rainbow_Add.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
			End
			EName Flash
			Part :RainbowFlash.part
			Part :RainbowFlash_Add.part
			Part :Flash_Small.part
			Part :SummonRing_Smoke_Add.part
			Part :Glow_Streaks.part
			LifeSpan 5
		End

	End

End
