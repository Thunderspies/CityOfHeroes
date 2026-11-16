#
FxInfo

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type PositOnly
			EName WhirlyA
			Bhvr behaviors/FadeOut.bhvr
			Part WORLD\City\DarkAstoria\Mot_Summon_Whirlwind.part
			Part WORLD\City\DarkAstoria\Mot_Summon_Whirlwind_Add.part
		End

		Event
			At T_Root
			Type PositOnly
			EName WhirlyB
			Bhvr behaviors/FadeOut.bhvr
			Part WORLD\City\DarkAstoria\Mot_Summon_Whirlwind2.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Root
			Type PositOnly
			EName Dustup
			Bhvr behaviors/FadeOut.bhvr
			Part WORLD\City\DarkAstoria\Mot_Summon_Dust.part
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Root
			Type PositOnly
			EName Runes
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				Spin 0 0.05 0
			End
			Part WORLD\City\DarkAstoria\Mot_Summon_RuneCircle.part
		End

	End

	Condition
		On Time
		Time 95

		Event
			At Root
			Type PositOnly
			EName Poof
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				Spin 0 0.05 0
			End
			Part WORLD\City\DarkAstoria\Mot_Summon_DustUp.part
			Part WORLD\City\DarkAstoria\Mot_Summon_DustUp_Add.part
			Part WORLD\City\DarkAstoria\Mot_Summon_DustOut.part
			Part WORLD\City\DarkAstoria\Mot_Summon_PartsOut.part
			Part WORLD\City\DarkAstoria\Mot_Summon_PartsOut_Add.part
			LifeSpan 8
		End

	End

End
