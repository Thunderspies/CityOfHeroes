#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo
#########################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	MALE

	Event
		Type	PositOnly
		At	Root
		WHILE	MALE
		Bhvr	:CracksTrail_CameraShake.bhvr
		Part	:Cracks_Radial.part
		Part	:Cracks_Radial_Continuing.part
		PArt	:Dust_Lingering.part
		PArt	:Dirt_Stream_Out.part
		Part	:InitRockScatter.part
	End
	##  sound here
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	FEMALE
	Event
		Type	PositOnly
		At	Root
		WHILE	FEMALE
		Bhvr	:CracksTrail_CameraShake.bhvr
		Part	:Cracks_Radial.part
		Part	:Cracks_Radial_Continuing.part
		PArt	:Dust_Lingering.part
		PArt	:Dirt_Stream_Out.part
		Part	:InitRockScatter.part
	End
	##  sound here
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	HUGE
	Event
		Type	PositOnly
		At	Root
		WHILE	HUGE
		Bhvr	:CracksTrail_CameraShake.bhvr
		Part	:Cracks_Radial.part
		Part	:Cracks_Radial_Continuing.part
		PArt	:Dust_Lingering.part
		PArt	:Dirt_Stream_Out.part
		Part	:InitRockScatter.part
	End
	##  sound here
End



#########################################################

End