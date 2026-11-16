#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Input
	InpName Hips
End

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
		Sound WWV_PlasmaEyeBeams_Attack 200 200 .8
	End

	Event
		EName	LeftEye
		Type	Local
		At	Core
		AltPiv	1
		Part	:Muzzle_Trail_Core.part
		Lifespan	50

	End

	Event
		EName	RightEye
		Type	Local
		At	Core
		AltPiv	2
		Part	:Muzzle_Trail_Core.part
		Lifespan	50


	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename 	Prime
		Type	Start
		At	LeftEye
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Beam_Head_Ember_Small.part
		Part	:Beam_Trail_Ember_Small.part
		Part	:Beam_Trail_Tendril_Small.part
		Part	:Beam_Trail_Glow_Small.part

		Magnet	Target
		LookAt	Target
		POther	Target

		Lifespan 35
	End

	Event
		Ename 	Prime
		Type	Start
		At	RightEye
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Beam_Head_Ember_Small.part
		Part	:Beam_Trail_Ember_Small.part
		Part	:Beam_Trail_Tendril_Small.part
		Part	:Beam_Trail_Glow_Small.part

		Magnet	Target
		LookAt	Target
		POther	Target

		Lifespan 35
	End

	Event
		EName 	BeamGeo
		Type	PositOnly
		At	LeftEye
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	0 0 255
			FadeInLength	5
			FadeOutLength	10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.35 .35 0.2
		End
		Part	:Beam_Glow_Small_Additive.part

		geom	Tintable_FireBlast1
		POther	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target

		LifeSpan		25
	End

	Event
		EName 	BeamGeo
		Type	PositOnly
		At	RightEye
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			StartColor	0 0 255
			FadeInLength	5
			FadeOutLength	10

			Scale		0.1 0.1 0.007
			ScaleRate	.05 .05 .05
 			EndScale	.35 .35 0.2
		End
		Part	:Beam_Glow_Small_Additive.part

		geom	Tintable_FireBlast1
		POther	Target
		PMagnet	Target
		Magnet	Target
		LookAt	Target

		LifeSpan		25
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End