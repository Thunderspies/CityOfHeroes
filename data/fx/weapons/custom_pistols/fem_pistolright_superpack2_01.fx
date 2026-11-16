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
		Anim	CustomWeapon_PistolRight_SuperPack2_01
		BhvrOverride
			Scale			0.7 0.7 0.7
			PositionOffset		0.03 0 -0.02
		End
	End
End

#############################################################

End