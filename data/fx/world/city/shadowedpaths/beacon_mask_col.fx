#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0

	Event
		EName	BeaconAnchor
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -4.0 0.0
		End
		Sound 	FirstWard_ShadowyOrb1_Loop 50 50 .15
		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
	End

	Event
		Type	Local
		At	BeaconAnchor
		BhvrOverride
			Alpha		255
			Scale		5.0 5.0 5.0
			PositionOffset	0.0 0.0 -0.05
		End
		Geom	GEO_Eye_CoL_Mask_01

	End

	Event
		Type	Local
		At	BeaconAnchor
		BhvrOverride
			Alpha		100
			StartColor	255 255 150
			Scale		5.0 5.0 5.0
			PositionOffset	0.0 0.0 -0.0475
		End
		Geom	GEO_Eye_CoL_Mask_ADD

	End

	Event
		EName	SpinAnchor
		Type	Local
		At	BeaconAnchor
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.02 0.0
		End
		Part	:Beacon_Border.part
		Part	:Beacon_Swirls.part
		Part	:Beacon_Swirls_Additive.part

	End

	Event
		EName	RightArm
		Type	Local
		At	SpinAnchor
		BhvrOverride
			PositionOffset	0 0 -1.5
			pyrRotate	0 90 0
		End
		Part	:Beacon_Wisps.part
		Part	:Beacon_Wisps_Glow.part

	End

	Event
		EName	LeftArm
		Type	Local
		At	SpinAnchor
		BhvrOverride
			PositionOffset	0 0 1.5
			pyrRotate	-0 -90 0
		End
		Part	:Beacon_Wisps.part
		Part	:Beacon_Wisps_Glow.part

	End

	Event
		EName	SpinAnchor
		Type	Local
		At	BeaconAnchor
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.01 2.0
		End
		Part	:Beacon_Core.part
		Part	:Beacon_Core_Glow.part
	End

End

Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	BeaconAnchor
		BhvrOverride
			PYRRotate	90 0 0
			Spin	0.0 0.015 0.0
		End
		Part	:Beacon_Particles_Stop.part
	End
End

#########################################################

End