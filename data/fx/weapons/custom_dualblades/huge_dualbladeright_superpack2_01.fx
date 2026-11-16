#############################################################
## Costume Anim Entity - Right Blade - 02
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
		Anim	CustomWeapon_BladeRight_SuperPack2_01
		BhvrOverride
			Scale			1.3 1.3 1.3
#			PositionOffset		0.1 0.02 0
		End
	End
End

#############################################################

End