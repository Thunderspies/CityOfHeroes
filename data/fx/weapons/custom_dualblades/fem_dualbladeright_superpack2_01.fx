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
		Anim	CustomWeapon_BladeRight_SuperPack2_01
		BhvrOverride
			Scale			0.8 0.8 0.8
#			PositionOffset		0.1 0.02 0
		End
	End
End

#############################################################

End