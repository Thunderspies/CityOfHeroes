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
		Bhvr	WEAPONS\Custom_Pistols\Huge_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_PistolRight_SuperPack2_01
		BhvrOverride
			Scale			1.3 1.3 1.3
			PositionOffset		-0.01 0 0
		End
	End
End

#############################################################

End