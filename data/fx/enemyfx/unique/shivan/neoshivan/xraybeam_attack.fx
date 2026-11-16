#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 150

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	ShivanNeo_X-RayBeam_Attack_01 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Attack_02 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Attack_03 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Attack_04 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Attack_05 200 200 .8
		Sound 	ShivanNeo_X-RayBeam_Attack_06 200 200 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	HandAnchorR
		Type	PositOnly
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Chest_Heartbeat.part
		Part	:Hands_Rays_Heavy.part
		Part	:Hands_Rings_Heavy.part
		Lifespan 90
	End


End

#############################################################

Condition
	On 	Time
	Time 	20


	Event
		Ename 	Prime
		Type	Start
		At	Neck
		Bhvr	CustomizeablePowers\DarkBlast\slowprjct5.bhvr
		BhvrOverride
			TrackRate		7.0
		End
		Part	:Beam_Head_Ember_Small.part
		Part	:Beam_Trail_Ember_Small.part
		Part	:Beam_Trail_Tendril_Small.part

		Magnet	Target
		LookAt	Target
		POther	Target

		Lifespan 35
	End

	Event
		EName 	BeamGeo
		Type	PositOnly
		At	Neck
		Bhvr	behaviors/InfernoBoltBeamSmall.bhvr
		BhvrOverride
			TintGeom	1
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