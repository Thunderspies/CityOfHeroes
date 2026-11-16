#############################################################
## Costume Anim Entity - Right Claws - 05
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
		Bhvr	WEAPONS\Custom_Claws\Male_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_ClawsRight_ViperBlades01
	End


	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Claws\Male_WepRScale.bhvr
		Anim	CustomWeapon_ClawsRight_ViperBlades01
	End
End

#############################################################

End