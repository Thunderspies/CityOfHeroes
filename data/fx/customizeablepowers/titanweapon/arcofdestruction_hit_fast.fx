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
		Sound 	TW_WaveOfDestruction_HitFast_01 100 75 .8
	End
End

Condition
	On Time
	Time 2

	Event
		At Root
		Type Local
		Bhvr behaviors/CameraShake01_Strong.bhvr
		BhvrOverride
			PositionOffset 0 1 0
		End
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
		At Root
		Type Local
		BhvrOverride
			PositionOffset 0 1 0
		End
		Part :Head_Shockwave_Small.part
		Part :Hit_Shockwave.part
		Part :Hit_Smoke.part
		Part :Hit_Smoke_Lingering.part
		Part :Hit_Streaks.part
		LifeSpan 5
	End

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset 0 1 0
		End
		Part :Head_Shockwave_Random.part
		LifeSpan 20
	End

	Event
		At Root
		Type Local
		BhvrOverride
			PositionOffset 0 0.5 0
		End
		Part	:Cracks_Hole2.part
		LifeSpan 20
	End

End

End
