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
		Anim	CustomWeapon_PistolRight_Dalton_Equalizer
		BhvrOverride
			Scale			0.8 0.8 0.8
			PositionOffset		0.1 0.01 -0.02
		End
	End
End

#############################################################

End