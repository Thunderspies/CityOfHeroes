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
		Part	:Sonic_Ripple_Large_FS.part
		Part	:Sonic_Glow_FS.part

	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake_Rumble.bhvr
	End
End

#############################################################

End