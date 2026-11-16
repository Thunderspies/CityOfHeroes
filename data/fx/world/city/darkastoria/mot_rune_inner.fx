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
		End

	End

End
