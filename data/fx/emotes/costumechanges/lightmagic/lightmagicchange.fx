#
FxInfo

	LifeSpan 130


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound DA_Tielekku_Summon_01 100 100 0.8
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
			Part :SummonRing_RingExpand.part
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
			Part :SummonRing_RingStreak.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			At SummonAnchor
			Type Local
			Part :SummonRing_Poof01.part
			Part :SummonRing_Poof02.part
			Part :SummonRing_Dust.part
			Part :SummonRing_Dust2.part
			Part :SummonRing_LightStreaks.part
		End

	End

	Condition
		On Time
		Time 90

		Event
			At SummonAnchor
			Type Local
			Part :SummonRing_Flash.part
			Part :SummonRing_Flash_Ground.part
			Part :SummonRing_FlareUp.part
		End

	End

End
