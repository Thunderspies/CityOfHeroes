#############################################################
## Costume Anim Entity - Right Blade - 03
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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScale.bhvr
		Anim	CustomWeapon_BladeLeft_Rapier_01
	End
End

#############################################################

End