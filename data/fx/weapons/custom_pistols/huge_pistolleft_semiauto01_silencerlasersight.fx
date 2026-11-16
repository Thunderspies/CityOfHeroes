#############################################################
## Costume Anim Entity - Left Pistol - 01
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
		Anim	CustomWeapon_PistolLeft_SemiAuto01_SilencerLaserSight
	End
End

#############################################################

End