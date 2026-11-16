#############################################################
## Costume Anim Entity - Right Blade - 05
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
		Anim	CustomWeapon_BladeLeft_ImperviumKatana_01
	End
End

#############################################################

End