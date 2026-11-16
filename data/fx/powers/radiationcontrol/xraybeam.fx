#############################################################
## HEADER
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
		BHVR	Behaviors\OffsetEyes.bhvr
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
End

Condition
	On	Time
	Time	0

	Event
		EName	TargetOffset
		Type	PositOnly
		At	T_Chest
		BHVR	Behaviors\OffsetEyes.bhvr
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
		Sound xray5a 80 80 .9
		Sound xray5b 80 80 .9
		Sound xray5c 80 80 .9
		Sound xray5d 80 80 .9
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	LeftEye
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\RadiationControl\XRayEyeGlow.Part
		Part2	POWERS\RadiationControl\XRayEyeRings.Part
		LifeSpan 6
	End

	Event
		Type	PositOnly
		At	RightEye
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\RadiationControl\XRayEyeGlow.Part
		Part2	POWERS\RadiationControl\XRayEyeRings.Part
		Sound radiation3_loop 80 80 .4
		LifeSpan 6
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	PositOnly
		At	LeftEye
		Bhvr	behaviors\XRayBeam.bhvr
		geom	XrayBeam2
		Magnet	TargetL
		LookAt	TargetL
		LifeSpan 12
	End

	Event
		Type	PositOnly
		At	RightEye
		Bhvr	behaviors\XRayBeam.bhvr
		geom	XrayBeam2
		Magnet	TargetR
		LookAt	TargetR
		LifeSpan 12
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	behaviors\Fastprojectile4.bhvr
		Magnet	T_Chest
		LookAt	T_Chest
	End
End

#############################################################

End