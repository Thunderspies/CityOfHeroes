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
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
		Anim	CustomWeapon_BladeRight_03
	End
End

#############################################################

End