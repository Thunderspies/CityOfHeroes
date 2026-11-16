#
FxInfo

	LifeSpan 100


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_TielekkuAOEBlast_Attack_01 100 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			#Sound CoL_RadiantBurst_Attack_01 100 100 0.8
		End

		Event
			At Chest
			Type Local
			EName RuneAnchor
			BhvrOverride
				PositionOffset 0 0 1
			End
			Part WORLD\City\DarkAstoria\BigRune.part
		End

		Event
			At WepR
			Type Local
			Part WORLD\City\DarkAstoria\HandRunes03.part
			Part WORLD\City\DarkAstoria\HandRunes04.part
			Part :StaffEnergyDots2.part
			LifeSpan 0
		End

		Event
			At WepL
			Type Local
			Part WORLD\City\DarkAstoria\HandRunes03.part
			Part WORLD\City\DarkAstoria\HandRunes04.part
			Part :StaffEnergyDots2.part
			LifeSpan 0
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Chest
			Type Local
			Part :RadiantFlare.part
			LifeSpan 5
		End

	End

	Condition
		On Time
		Time 45

		Event
			Type Start
			EName ScreenFlash
			Part :Radiant_Flash.part
		End

	End

End
