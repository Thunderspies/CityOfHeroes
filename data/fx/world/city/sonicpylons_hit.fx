#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		CAMERASPACE
		Bhvr	behaviors/CameraShake_Rumble.bhvr
		Part	:Sonic_Rings_Blast.part
		Part	:Sonic_Blast.part
		Part	:Sonic_Noise_Linger.part

		Lifespan 15
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake03.bhvr

	End
End

#############################################################

End