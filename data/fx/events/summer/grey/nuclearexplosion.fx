#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Sound Nuclear_Explosion_01 200 200 .9
		Lifespan	120
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	WepR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	FootR
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LArmL
	End


	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LArmR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow.part
		Lifespan	120
		POther	LLegR
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Nuke_Body_Glow_Big.part
		Lifespan	120
		POther	Hips
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShakeHydra.bhvr
		BhvrOverride
			ShakeFallOff	0.005
		End
		Part	:Nuke_Flash.part
		Part	:Nuke_Glow_Rising.part
		Part	:Nuke_Fire_Rising.part
		Part	:Nuke_Fire_Rising_Lingering.part
		Part	:Nuke_Fire_Rising_Column.part
		Part	:Nuke_Shockwave.part
		Lifespan	12
	End

	Event
		Type	Start
		At	Root
		Part	:Nuke_Smoke_Cloud.part
		Lifespan	30
	End

	Event
		EName	RingAnchor
		Type	Local
		At	Root
		Bhvr	:Ground_Dust_Expand.bhvr
		BhvrOverride
			PositionOffset	0 3.5 0
		End
		Part	:Nuke_Smoke_Ring_Expanding.part
		Part	:Nuke_Fire_Ring_Expanding.part

		Lifespan	150
	End

	Event
		Type	Local
		At	Root
		Part	:Nuke_Smoke_Ring_Central.part
		Part	:Nuke_Shockwave_Flat.part
		Lifespan	20
	End

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Camera_Dark.part
		Part	:Camera_Dark_Clouds.part
		Lifespan	10
	End

	Event
		EName	Sigil
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	5
			FadeOutLength	120
			StartColor	50 50 50
			Scale		80.0 15.0 80.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 100
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	StartPositOnly
		At	Root
		Part	:Nuke_Smoke_Column.part
		Part	:Nuke_Smoke_Cloud_Lingering.part

		Lifespan	45
	End

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:CAMERA_Debris.part

		Lifespan	40
	End

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:CAMERA_Dust.part

		Lifespan	80
	End

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:CAMERA_Dust.part

		Lifespan	60
	End

End

Condition
	On	Time
	Time	45

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 -15 0
		End
		Part	:Nuke_Smoke_Column_Lingering.part

		Lifespan	75
	End
End

Condition
	On	Time
	Time	55

	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Camera_Dust_Still.part
		Lifespan	30
	End
End

#############################################################

End
