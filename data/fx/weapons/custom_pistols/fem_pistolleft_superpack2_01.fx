#############################################################
## Costume Anim Entity - Left Pistol - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Pistols\Female_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_PistolRight_SuperPack2_01
		BhvrOverride
			Scale			0.7 0.7 0.7
			PositionOffset		0.06 0 -0.02
		End
	End
End

#############################################################

End