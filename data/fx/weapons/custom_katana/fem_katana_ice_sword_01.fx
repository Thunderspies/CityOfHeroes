#############################################################
## Costume Anim Entity - Broadsword 05
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Katana\Female_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_Ice_Sword_01
	End
End

#############################################################

End