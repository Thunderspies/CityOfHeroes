#
FxInfo

Flags DontSuppress

###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_MotRune_Loop 100 100 .5
		lifespan 120
	End
End

Condition
	On 	Time
	Time	120
	Event
		Type	Local
		At	Root
		Sound 	DA_MotRuneDeath_01 100 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName RuneAnchor
			BhvrOverride
				PositionOffset 0 5 5
			End
			Part WORLD\City\DarkAstoria\Rune_Inner_Smoke_Add.part
			Part WORLD\City\DarkAstoria\Rune_Inner_Smoke_Subtractive.part
			Part WORLD\City\DarkAstoria\Rune_Inner_Smoke_Alpha.part
			Part WORLD\City\DarkAstoria\Rune_Inner01.part
			Part WORLD\City\DarkAstoria\Rune_Inner02.part
			Part WORLD\City\DarkAstoria\Rune_Inner_PowerBlobs.part
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 120

		Event
			At Root
			Type Local
			EName RuneAnchor
			BhvrOverride
				PositionOffset 0 5 5
			End
			Part WORLD\City\DarkAstoria\RuneSplode_Lightshaft02.part
			Part WORLD\City\DarkAstoria\RuneSplode_Lightshaft03.part
			Part WORLD\City\DarkAstoria\RuneSplode_Streaks.part
			Part WORLD\City\DarkAstoria\RuneSplode_Streaks_Detached.part
			Part WORLD\City\DarkAstoria\RuneSplode_Swirls.part
			Part WORLD\City\DarkAstoria\RuneSplode_Swirls_Additive.part
			Part WORLD\City\DarkAstoria\RuneSplode_Whirl.part
			Part WORLD\City\DarkAstoria\RuneSplode_Lightshaft01.part
			LifeSpan 5
		End

	End

End
