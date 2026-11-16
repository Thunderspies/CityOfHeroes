#############################################################
## Costume Anim Entity - Right Blade - Base
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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_BladeLeft_BlackKnight_01
	End
End

#############################################################

End