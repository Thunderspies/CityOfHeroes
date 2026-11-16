#############################################################
## Costume Anim Entity - Right Pistol - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Pistols\Female_WepRScale.bhvr
		Anim	CustomWeapon_PistolRight_SemiAuto01_Silencer
	End
End

#############################################################

End