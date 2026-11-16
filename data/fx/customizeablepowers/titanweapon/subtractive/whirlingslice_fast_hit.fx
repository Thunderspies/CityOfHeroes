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
		Sound 	TW_WhirlingSlice_HitFast_01 100 75 .8
	End
End

Condition
	On Time
	Time 2

	Event
		At Chest
		Type Local
		Bhvr behaviors/CameraShake01_Strong.bhvr
		Part :Head_Spikes.part
		Part :Hit_Flash_Big.part
		Part :Hit_Sparks.part
		Part :Hit_Glow_Lingering.part
		LifeSpan 60
	End

End

Condition
	On Time
	Time 4

	Event
		At Chest
		Type Local
		Part :Head_Shockwave_Small.part
		Part :Hit_Shockwave.part
		Part :Hit_Smoke.part
		Part :Hit_Streaks.part
		LifeSpan 5
	End

	Event
		At Chest
		Type Local
		Part :Head_Shockwave_Random.part
		LifeSpan 20
	End

End

End
