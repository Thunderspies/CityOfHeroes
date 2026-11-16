#############################################################
## Costume Anim Entity - Right Blade - 04
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
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepRScaleAndFlip.bhvr
		Anim	CustomWeapon_BladeLeft_Katana_04
	End
End

#############################################################

End