#############################################################
## FifthCol_Portal.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offsets
		Type	Start
		At	Root
		geom	GEO_5thColumnPortalOffsets
		Sound   Apparitions_CrushingFate_Loop 100 100 .8
	End

#############################################################
###	PORTAL		  ###################################
#############################################################

	Event
		EName	Offset
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Oval.part
		Part	:Oval_Fill_Alpha.part
		Part	:Oval_Shrinking.part
		Part	:Core_Tendrils.part
	End


End