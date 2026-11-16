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
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
		Anim	CustomWeapon_BladeRight_Valkyrie_01
	End
End

#############################################################

End