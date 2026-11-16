#############################################################
## Costume Anim Entity - Katana - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Katana_NinjaBlade_01
	End
End

#############################################################

End