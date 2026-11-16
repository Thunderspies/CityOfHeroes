#############################################################
## Costume Anim Entity - Left Blade - 03
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
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_BladeRight_03
	End
End

#############################################################

End