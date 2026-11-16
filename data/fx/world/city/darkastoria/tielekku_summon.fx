#
FxInfo

	LifeSpan 130


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_Tielekku_Summon_01 100 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName SummonAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				Spin 0 0.045 0
			End
			Part WORLD\City\DarkAstoria\SummonRing_RingExpand.part
			Part :SummonCircle_Runes.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part WORLD\City\DarkAstoria\SummonRing_RingStreak.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At SummonAnchor
			Type Local
			Part WORLD\City\DarkAstoria\SummonRing_Poof01.part
			Part WORLD\City\DarkAstoria\SummonRing_Poof02.part
			Part WORLD\City\DarkAstoria\SummonRing_Dust.part
			Part WORLD\City\DarkAstoria\SummonRing_Dust2.part
			Part WORLD\City\DarkAstoria\SummonRing_LightStreaks_Small.part
		End

	End

	Condition
		On Time
		Time 90

		Event
			At SummonAnchor
			Type Local
			Part WORLD\City\DarkAstoria\SummonRing_Flash.part
			Part WORLD\City\DarkAstoria\SummonRing_Flash_Ground.part
			Part WORLD\City\DarkAstoria\SummonRing_FlareUp.part
		End

	End

End
