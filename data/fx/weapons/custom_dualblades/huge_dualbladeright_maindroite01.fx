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
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepRScale.bhvr
		Anim	CustomWeapon_BladeLeft_01
	End
End

#############################################################

End