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
		EName	LeftEye
		Type	Local
		At	WepL
	End

	Event
		EName	RightEye
		Type	Local
		At	WepR
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
		At	root
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
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Cust_XRayEyeGlow.Part
		Part2	CustomizeablePowers\RadiationBlast\Cust_XRayEyeRings.Part
		LifeSpan 6
	End

	Event
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\RadiationBlast\Cust_XRayEyeGlow.Part
		Part2	CustomizeablePowers\RadiationBlast\Cust_XRayEyeRings.Part
		Sound radiation3_loop 80 80 .4
		LifeSpan 6
	End
End

#############################################################

Condition
	On	Time
	Time	23

	Event
		Type	PositOnly
		At	WepL
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
		At	WepR
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