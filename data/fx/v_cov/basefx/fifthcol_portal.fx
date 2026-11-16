#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		geom	_Fifthcol_Portal_Active
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offsets
		Type	Start
		At	Root
		geom	GEO_5thColumnPortalOffsets
	End

	Event
		EName	Offsets
		Type	Start
		At	Offsets
		AltPiv	1
	End

	Event
		EName	Offsets
		Type	Start
		At	Offsets
		AltPiv	2
		ChildFX	ENEMYFX\Rikti\RiktiPortalStabilizer\RiktiPortalStabilizer_SmallLightningArc1.fx

	End
End

