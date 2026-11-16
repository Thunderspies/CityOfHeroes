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
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	PositOnly
		At	LeftEye
		Bhvr	POWERS\RadiationControl\XRayBeam.bhvr
		Part	POWERS\RadiationControl\XRayEyeGlowRed2.Part
		Part	POWERS\RadiationControl\XRayEyeRingsRed2.Part
		LifeSpan 6
	End

	Event
		Type	PositOnly
		At	RightEye
		Bhvr	POWERS\RadiationControl\XRayBeam.bhvr
		Part	POWERS\RadiationControl\XRayEyeGlowRed2.Part
		Part	POWERS\RadiationControl\XRayEyeRingsRed2.Part
		LifeSpan 6
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	LeftEye
		Bhvr	behaviors\XRayBeam2.bhvr
		geom	XrayBeamRed
		Magnet	TargetL
		LookAt	TargetL
		LifeSpan 26
	End

	Event
		Type	PositOnly
		At	RightEye
		Bhvr	behaviors\XRayBeam2.bhvr
		geom	XrayBeamRed
		Magnet	TargetR
		LookAt	TargetR
		LifeSpan 26
	End
End

#############################################################

End