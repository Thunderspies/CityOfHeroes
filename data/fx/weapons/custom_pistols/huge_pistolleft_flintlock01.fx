#############################################################
## Costume Anim Entity - Left Pistol - 03
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
		ChildFX	:FX_PistolRight_Flintlock_01.fx
	End
End

#############################################################

End