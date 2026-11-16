#############################################################
## AbyssalMending_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	90
	Chance	0.25

	Event
		EName	WaterRipples
		Type	Local
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Ripples_Pool.part
		Lifespan 25
	End

	Event
		EName	CAMERA_Flashes
		Type	PositOnly
		CAMERASPACE
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Flashes_Camera.part
		Lifespan 1
	End

	Event
		EName	CAMERA_Shake
		Type	PositOnly
		Bhvr	behaviors/CameraShake05Subtle.bhvr
		BhvrOverride
			Shake          0.75     #How much to shake the camera when event with this bhvr is created
			ShakeFallOff   .0175    #how long shake lasts
			ShakeRadius    500
		End
		SoundNoRepeat 3
		Sound DestroyedOuroboros_Thunder_01 500 100 .9
		Sound DestroyedOuroboros_Thunder_02 500 100 .9
		Sound DestroyedOuroboros_Thunder_03 500 100 .9
		Sound DestroyedOuroboros_Thunder_04 500 100 .9
		Sound DestroyedOuroboros_Thunder_05 500 100 .9
		Lifespan 1
	End
End

#############################################################

End


