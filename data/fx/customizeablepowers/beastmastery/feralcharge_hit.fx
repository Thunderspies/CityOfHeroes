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

		Event
			At Hips
			Type Local
			Part :FeralCharge_Streaks.part
			Part :FeralCharge_Flare.part
			Part :Hit_Smoke_Additive.part
			Part :Hit_Smoke.part
			LifeSpan 10
		End


	End

###########################################################

End
