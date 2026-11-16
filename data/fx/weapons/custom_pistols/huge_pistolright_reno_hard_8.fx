#############################################################
## Costume Anim Entity - Right Pistol - Base
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
		Bhvr	WEAPONS\Custom_Pistols\Female_WepRScale.bhvr
		Anim	CustomWeapon_PistolRight_Reno_Hard_8
		BhvrOverride
			Scale			1.2 1.2 1.2
			PositionOffset		0 0 0.1
		End
	End
End

#############################################################

End