#############################################################
## NeuronsTower_Beam.fx
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MidAnchor
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -165.0 0.0
		End
	End


	Event
		EName	Anchor
		Type	Start
		At	Root
	End

	Event
		EName	TopAnchor
		Type	Start
		At	Anchor
		BhvrOverride
			PositionOffset	0.0 170.0 0.0
		End
		POther	Anchor
		LookAt	Anchor
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Part	:Projectile_Trail_Glow_Outer.part
	End


End

Condition
	On	Time
	Time	60

	Event
		Type	Start
		At	Anchor
		Part	:Projectile_Trail_WhiteNoise.part
		Lifespan	1
		POther	TopAnchor
		LookAt	TopAnchor
	End
End

Condition
	On	Time
	Time	75

	Event
		Type	Start
		At	Anchor
		Part	:Projectile_Trail_Electricity_Warning.part
		Part	:Projectile_Trail_Electricity_Warning2.part
		Lifespan	105
		POther	TopAnchor
		LookAt	TopAnchor
	End
End

Condition
	On	Time
	Time	150



	Event
		Bhvr	:CameraShake_JumpSettle.bhvr
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.0 0.0
		End
		Part	:OrbitalBeam_Flash.part
		Part	:OrbitalBeam_ImpactFlash.part
		Lifespan	40
	End

	Event
		EName	RingAnchor
		Type	Local
		At	Root
		Bhvr	:Ground_Dust_Expand_Quick.bhvr
		BhvrOverride
			PositionOffset	0 3.5 0
		End
		Part	:OrbitalBeam_Smoke_Ring_Expanding.part
		Part	:OrbitalBeam_Fire_Ring_Expanding.part

		Lifespan	90
	End

	Event
		Type	Start
		At	Root
		Part	:OrbitalBeam_Shockwave.part
		Lifespan	90
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Part	:Projectile_Trail_Core.part
		Part	:Projectile_Trail_Core_Alpha.part
		Part	:Base_Splash.part
		Part	:Base_Splash_Core.part
		Lifespan	80
		POther	TopAnchor
		LookAt	TopAnchor
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Part	:Projectile_Trail_Electricity.part
		Part	:Projectile_Trail_Electricity2.part
		Part	:Projectile_Trail_Glow.part
		Lifespan	90
		POther	TopAnchor
		LookAt	TopAnchor
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Base_Splash_Flat.part
		Part	:Base_Shockwaves_Flat.part
		Part	:Base_Shockwave_Flat.part
		Lifespan	95

	End

End

Condition
	On	Time
	Time	157
	Event
		Type	Start
		CAMERASPACE
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Camera_Flash.part
		Lifespan	10
	End

	Event
		Type	StartPositOnly
		At	Anchor
		Part	:Projectile_Trail_Flash.part
		Part	:Screen_Flash.part

		Lifespan	5
		POther	TopAnchor
		LookAt	TopAnchor
	End
End

Condition
	On	Time
	Time	240

	Event
		Type	Start
		At	Anchor
		Part	:Projectile_Trail_Electricity_Warning.part
		Part	:Projectile_Trail_Electricity_Warning2.part
		Lifespan	30
		POther	TopAnchor
		LookAt	TopAnchor
	End
End

#############################################################

End