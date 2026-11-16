#
FxInfo

Lifespan 60
###########################################################

Condition
	On 	Time
	Time	0

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
End


Condition
	On 	Time
	Time 	0
	Random	1

		Event
			EName ElectricityAnchor
			At LLegR
			Type Local
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End

		Event
			EName ElectricityAnchor2
			At LLegL
			Type Local
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End
	End
End
