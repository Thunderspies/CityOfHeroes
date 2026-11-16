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
			Scale	0.6875 0.6875 0.6875
		End
		Anim	CustomWeapon_ClawsRight_FlatBlade01
	End
End

#############################################################

End