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
			Scale			1.2 1.2 1.2
			PositionOffset		-0.01 0 0.1
		End
	End
End

#############################################################

End