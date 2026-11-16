#############################################################
## Costume Anim Entity - Bow Base
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
		Bhvr	WEAPONS\Custom_Bow\Female_WepLScale.bhvr
		Anim	CustomWeapon_Longbow_01
	End
End

#############################################################

End