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
		Sound 	Brawling_SpinningStrike_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Head
		Bhvr	behaviors/CameraShake02.bhvr #behaviors/CameraShake_Strong20.bhvr
		Part	:Head_Shockwave_Small.part
		Part	:Head_Spikes.part

		LifeSpan 2
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	Head
		Bhvr	behaviors/CameraShake03.bhvr #behaviors/CameraShake_Strong20.bhvr
		Part	:Head_Shockwave_Small.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Smoke_Additive.part
		Part	:Hit_Smoke_Lingering.part
		Part	:Head_Spikes.part
		Part	:Hit_Flash_Big.part
		LifeSpan 6
	End
End

#############################################################

End