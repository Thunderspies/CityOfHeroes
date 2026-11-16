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
		Anim	CustomWeapon_PistolRight_Reno_Hard_8
		BhvrOverride
			Scale			0.8 0.8 0.8
			PositionOffset		0.15 0.01 -0.06
		End
	End
End

#############################################################

End