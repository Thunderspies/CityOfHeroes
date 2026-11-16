#############################################################
## Costume Anim Entity - Left Pistol - 02
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
		Anim	CustomWeapon_PistolRight_SW_Model29_01
	End
End

#############################################################

End