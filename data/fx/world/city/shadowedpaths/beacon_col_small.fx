##############################################################
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
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	FirstWard_ShadowyOrb2_Loop 35 35 .15
		Sound 	FirstWard_ShadowyOrb3_Loop 35 35 .15
		Sound 	FirstWard_ShadowyOrb4_Loop 35 35 .15
		Sound 	FirstWard_ShadowyOrb5_Loop 35 35 .15
		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
	End
End

Condition
	On 	Time
	Time	0

	Event
		EName	MaskAncor
		Type	Local
		At	Hips
		BhvrOverride
			Alpha		255
			Scale		5.0 5.0 5.0
			PositionOffset	0.0 -4.0 -1.0
		End
	End

	Event
		Type	Local
		At	MaskAncor
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.02 0.0
		End
		Part	:Beacon_Border_Small.part
		Part	:Beacon_Swirls_Small.part
		Part	:Beacon_Swirls_Additive_Small.part

	End


	Event
		EName	SpinAnchor
		Type	Local
		At	MaskAncor
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 5.01 0.0
		End
		Part	:Beacon_Core_Small.part
		Part	:Beacon_Core_Glow.part
	End

End

#########################################################

End