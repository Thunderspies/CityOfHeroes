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
		Bhvr	WEAPONS\Custom_Pistols\Huge_WepRScale.bhvr
		Anim	CustomWeapon_PistolRight_SemiAuto01
	End
End

#############################################################

End