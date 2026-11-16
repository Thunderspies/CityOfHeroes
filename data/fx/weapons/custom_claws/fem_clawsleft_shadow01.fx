#############################################################
## Costume Anim Entity - Left Claws - 05
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
		Anim	CustomWeapon_ClawsRight_ViperBlades01
	End
End

#############################################################

End