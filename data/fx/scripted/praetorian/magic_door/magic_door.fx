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
		Type	Start
		At	Root
		BhvrOverride
			Alpha		255
			Scale		5.0 5.0 5.0
			PositionOffset	0.0 0.0 -1.0
		End
		Sound 	Apparitions_CrushingFate_Loop 100 100 .8
	End

	Event
		EName	CrushingHateSwirls
		Type	Start
		At	Root
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.02 0.0
		End
		Part	:Beacon_Border.part
		Part	:Beacon_DarkSpot.part
		Part	:Beacon_Swirls_Additive.part

	End

	Event
		EName	RightArm
		Type	Local
		At	CrushingHateSwirls
		BhvrOverride
			PositionOffset	1.5 0 0
			pyrRotate	0 0 0
		End
		Part	:Beacon_Wisps.part
		Part	:Beacon_Wisps_Glow.part

	End

	Event
		EName	LeftArm
		Type	Local
		At	CrushingHateSwirls
		BhvrOverride
			PositionOffset	-1.5 0 0
			pyrRotate	0 180 0
		End
		Part	:Beacon_Wisps.part
		Part	:Beacon_Wisps_Glow.part
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	Start
		BhvrOverride
			PYRRotate	0 0 0
			Spin	0.0 0.015 0.0
		End
		At	CrushingHateSwirls
		Part	:Beacon_Particles_Stop.part
	End
End

#########################################################

End