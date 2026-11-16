#############################################################
## Costume Anim Entity - Left Claws - Base
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
		Bhvr	WEAPONS\Custom_Claws\Female_WepLScaleAndFlip.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_ClawsRight_VillainBase
	End
End

#############################################################

End