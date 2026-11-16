#
FxInfo

Lifespan 60
###########################################################

	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	BM_Hit_01 100 100 .5
		Sound 	BM_Hit_02 100 100 .5
		Sound 	BM_Hit_03 100 100 .5
		Sound 	BM_Hit_04 100 100 .5
		Sound 	BM_Hit_05 100 100 .5
		Sound 	BM_Hit_06 100 100 .5
		Sound 	BM_Hit_07 100 100 .5
	End

		Event
			EName	SlashAnchor
			At Chest
			Type Local
			LifeSpan 40
		End

		Event
			At Chest
			Type Local
			Part :Hit_Flash.part
			Part :Hit_Glow_Additive.part
			LifeSpan 10
		End

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End
	End

	Condition
		On Time
		Time 10
	Event
		Type	Local
		At	Root
		Sound 	BM_Hit_08 100 100 .4
	End
		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End
	End


	Condition
		On Time
		Time 15
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	BM_Hit_01 100 100 .5
		Sound 	BM_Hit_02 100 100 .5
		Sound 	BM_Hit_03 100 100 .5
		Sound 	BM_Hit_04 100 100 .5
		Sound 	BM_Hit_05 100 100 .5
		Sound 	BM_Hit_06 100 100 .5
		Sound 	BM_Hit_07 100 100 .5
	End
		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End
	End

	Condition
		On Time
		Time 30
	Event
		Type	Local
		At	Root
		Sound 	BM_Hit_09 100 100 .6
	End
		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash_Large.part
			Part :Hit_Slash_Large.part
			LifeSpan 10
		End
	End
End
