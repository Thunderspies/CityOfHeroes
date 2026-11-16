#############################################################
## Costume Anim Entity - Left Blade - 04
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
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepLScale.bhvr
		Anim	CustomWeapon_BladeLeft_Katana_01
	End
End

#############################################################

End