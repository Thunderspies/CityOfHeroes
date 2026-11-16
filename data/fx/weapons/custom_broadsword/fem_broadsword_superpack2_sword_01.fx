#############################################################
## Costume Anim Entity - Left Blade - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_SuperPack2_Sword_01
		BhvrOverride
			Scale			0.8 0.8 0.8
#			PositionOffset		0.1 0.02 0
		End
	End
End

#############################################################

End