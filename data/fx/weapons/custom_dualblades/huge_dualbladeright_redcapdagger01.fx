#############################################################
## Costume Anim Entity - Right Blade - 01
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
		Anim	CustomWeapon_BladeRight_RedcapDagger_01
	End
End

#############################################################

End