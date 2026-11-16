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
	End


End


#############################################################

End