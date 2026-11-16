#############################################################
## Costume Anim Entity - Assault Rifle - Base
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
		Bhvr	WEAPONS\Custom_AssaultRifle\Huge_WepRScale.bhvr
		Anim	CustomWeapon_AssaultRifle_Arachnos01B
	End
End

#############################################################

End