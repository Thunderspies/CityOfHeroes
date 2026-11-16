#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo
#########################################################

Condition
	On		Time
	Time		0

	Event
		Type	PositOnly
		At	Root
		WHILE	MALE
		Bhvr	:CracksTrail_CameraShake.bhvr
		Part	:Cracks_Radial.part
	#	Part	:Cracks_Radial_Continuing.part
		PArt	:Dust_Lingering.part
		PArt	:Dirt_Stream_Out.part
		Part	:InitRockScatter.part
	End
End

#########################################################

End