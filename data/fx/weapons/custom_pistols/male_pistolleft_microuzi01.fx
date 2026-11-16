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
		Bhvr	Behaviors/Reverse_Wep.bhvr
		Anim	CustomWeapon_PistolRight_MicroUzi
	End
End

#############################################################

End