#############################################################
## Costume Anim Entity - Bow 01
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
		Bhvr	WEAPONS\Custom_Bow\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Bow_01
	End
End

#############################################################

End