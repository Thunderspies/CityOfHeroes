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
		Anim	CustomWeapon_ClawsRight_Hero02
	End
End

#############################################################

End