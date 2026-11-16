#############################################################
## Costume Anim Entity - Long Staff Test
#############################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		Anim	CustomWeapon_LongStaff_Studded
		BhvrOverride
#			Scale			0.7 0.7 0.7
			PositionOffset		-0.1 -0.02 0
#			PYRrotate 		0 -6 0
# 			PYRrotateJitter 	0 0 0
		End
	End

End

#############################################################

End