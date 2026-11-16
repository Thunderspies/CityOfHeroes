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
		Anim	FX_Backpack_Elemental_Order_01
		BhvrOverride
			Scale			1.2 1.2 1.2
			PositionOffset		0 0 -0.4
#			PYRrotate 		0 0 0
# 			PYRrotateJitter 	0 0 0
		End
	End


End


#############################################################

End