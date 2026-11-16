###########################################################

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
		On Time
		Time 0
		Random	1

		Event
			At LLegL
			Type Local
			Part :ViciousBite_Streaks_Shrinking.part
			Part :ViciousBite_Flare_Shrinking.part
			Part :ViciousBite_Sparks.part

			LifeSpan 10
		End

		Event
			At LLegR
			Type Local
			Part :ViciousBite_Streaks_Shrinking.part
			Part :ViciousBite_Flare_Shrinking.part
			Part :ViciousBite_Sparks.part

			LifeSpan 10
		End
	End

###########################################################

End
