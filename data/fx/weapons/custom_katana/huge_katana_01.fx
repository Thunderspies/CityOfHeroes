#############################################################
## Costume Anim Entity - Katana - 01
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
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Katana_01
	End
End

#############################################################

End