#############################################################
## Costume Anim Entity - Backpack
#############################################################

FxInfo

Flags InheritAlpha DontSuppress //InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Back
		Anim	FX_Backpack_SuperPack2_01
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 0 0
#			PYRrotate 		0 0 0
# 			PYRrotateJitter 	0 0 0
		End
		ChildFX	:Male_SuperPack2_Neck_01_CHILD.fx
	End

#	Event
#		Type	Local
#		At	Chest
#		BhvrOverride
#//			Scale		0.7 0.7 0.7
#			TintGeom	1
#		End
#		GEOM	FX_Chest_SuperPack2_01_Male
#	End

End


#############################################################

End