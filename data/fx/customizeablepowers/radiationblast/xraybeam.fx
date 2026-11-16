#############################################################
## XrayBeam.fx
#############################################################

FxInfo
LifeSpan 60
#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Core
		Type	Local
		At	Eyes
		BHVR	CustomizeablePowers\RadiationBlast\OffsetEyes.bhvr
		Geom	CircleOfThorns
	End

	Event
		EName	LeftEye
		Type	Local
		At	Core
		AltPiv	1
	End

	Event
		EName	RightEye
		Type	Local
		At	Core
		AltPiv	2
	End

	Event
		EName	TargetOffset
		Type	PositOnly
		At	T_Chest
		BHVR	CustomizeablePowers\RadiationBlast\OffsetEyes.bhvr
		Geom	CircleOfThorns
		LookAt	Eyes
	End

	Event
		EName	TargetL
		Type	Local
		At	TargetOffset
		AltPiv	2
	End

	Event
		EName	TargetR
		Type	Local
		At	TargetOffset
		AltPiv	1
	End
End

#############################################################

Condition
	On	time
	time	6

	Event
		Type	local
		At	core
		Sound xray5 80 80 .9
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	LeftEye
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\XRayEyeGlow.Part
		Part2	CustomizeablePowers\RadiationBlast\XRayEyeRings.Part
		LifeSpan 6
	End

	Event
		Type	PositOnly
		At	RightEye
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\XRayEyeGlow.Part
		Part2	CustomizeablePowers\RadiationBlast\XRayEyeRings.Part
		Sound radiation3_loop 80 80 .4
		LifeSpan 6
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		Type	PositOnly
		At	LeftEye
		Bhvr	CustomizeablePowers\RadiationBlast\XRayBeam.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_XrayBeam2
		Magnet	TargetL
		LookAt	TargetL
		LifeSpan 12
	End

	Event
		Type	PositOnly
		At	RightEye
		Bhvr	CustomizeablePowers\RadiationBlast\XRayBeam.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_XrayBeam2
		Magnet	TargetR
		LookAt	TargetR
		LifeSpan 12
	End
End

#############################################################

End