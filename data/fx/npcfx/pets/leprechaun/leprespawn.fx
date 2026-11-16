#
FxInfo

	LifeSpan 500


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
			Part :SummonRing_Poof01.part
			Part :SummonRing_Poof02.part
			Part :SummonRing_Dust.part
			Part :SummonRing_Dust2.part
			Part :SummonRing_LightStreaks_Small.part
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			EName BeginAnchor
			BhvrOverride
				PositionOffset -15 60 0
			End
		End

		Event
			At Root
			Type Local
			EName EndAnchor
			BhvrOverride
				PositionOffset -0.25 2.5 0
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
		Time 36

		Event
			At Root
			Type Local
			EName Flash
			Part :Flash_Small.part
			Part :Clover_Streaks.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName Flash
			Part :RainbowFlash.part
			Part :RainbowFlash_Add.part
			Part :Spawn_RainbowFlash.part
			Part :Spawn_Flash.part
			Part :SummonRing_GreenDust.part
			Part :SummonRing_GreenDust_Add.part
			Part :Glow_Streaks.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 40
		Repeat 45

		Event
			At Root
			Type Start
			EName Flash
			BhvrOverride
				Drag 0.01
				Gravity 0.0260
				InitialVelocity 0 0.4 0
				InitialVelocityJitter 0.2 0.25 0.2
				PyrRotateJitter 180 0 180
				SpinJitter 0.10 0 0.10
			End
			Part :Clover_Splosion.part
		End

	End

End
