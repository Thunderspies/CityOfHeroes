#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Brawling_CrushingUppercut_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	2
	Event
		Type	local
		At	Head
		Bhvr	behaviors/CameraShake01_Strong.bhvr #behaviors/CameraShake_Strong20.bhvr
		Part	:Head_Spikes.part
		Part	:Hit_Flash_Big.part
		Part	:Hit_Sparks.part
		Part	:Hit_Glow_Lingering.part
		LifeSpan 60
	End
End

Condition
	On	Time
	Time	4

	Event
		Type	local
		At	Head
		Part	:Head_Shockwave_Small.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Smoke.part
		Part	:Hit_Smoke_Lingering.part
		Part	:Hit_Streaks.part
		LifeSpan 5
	End

	Event
		Type	local
		At	Head
		Part	:Head_Shockwave_Random.part
		Lifespan 20
	End
End

#############################################################

End