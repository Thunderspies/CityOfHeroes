#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	1
	Event
		Type	Local
		At	Root
		Sound 	Brawling_SweepingCross_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	1

	Event
		Type	Start
		At	Head
		Bhvr	behaviors/CameraShake03.bhvr #behaviors/CameraShake_Strong20.bhvr
		Part	:Head_Shockwave_Small.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Smoke_Additive.part
		Part	:Hit_Smoke_Lingering.part
		Part	:Head_Spikes.part
		part	:Hit_Streaks.part
		Part	:Hit_Flash_Big.part
		LifeSpan 6
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	local
		At	Head
		Part	:Head_Shockwave_Random.part
		Lifespan 20
	End
End

#############################################################

End