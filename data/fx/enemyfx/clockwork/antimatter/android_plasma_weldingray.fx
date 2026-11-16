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
		Type	Local
		At	Root
		sound Plasma_Welding_Ray 200 200 .8
	End

	Event
		Ename	Flash
		Type	Local
		At	C_Neck
		Part	:Muzzle_Strobe_Core.part
		Part	:Muzzle_Strobe_Glow.part
		Part2	:Muzzle_Smoke_Rapid.part

		Lifespan 40
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename	Flash
		Type	PositOnly
		At	C_Neck

		Part	:Muzzle_Trail_Core.part

		Lookat 	Target
		PMagnet	Target

		Lifespan 80

	End
End

#############################################################

Condition
	On 	Time
	Time 	40



	Event
		Ename 	Prime
		Type	Start
		At	C_Neck
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		7.0
		End
	#	Part	:Beam_Head_Ember_Small.part
	#	Part	:Beam_Trail_Ember_Small.part
	#	Part	:Beam_Trail_Tendril_Small.part
	#	Part	:Projectile_Trail_Glow_Small.part

		Magnet	Target
		LookAt	Target
		POther	Target

		Lifespan 80
	End

	Event
		EName 	BeamGeo
		Type	PositOnly
		At	C_Neck
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

		LifeSpan		65
	End
End

Condition
	On	Time
	Time	95

	Event
		Ename 	Prime
		Type	Start
		At	C_Neck
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		7.0
		End

		Part	:Beam_Trail_Steam.part

		Magnet	Target
		LookAt	Target
		POther	Target

		Lifespan 10
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