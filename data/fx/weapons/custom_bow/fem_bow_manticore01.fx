#############################################################
## Costume Anim Entity - Bow 02
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
		Anim	CustomWeapon_Bow_03
	End
End

#############################################################

End