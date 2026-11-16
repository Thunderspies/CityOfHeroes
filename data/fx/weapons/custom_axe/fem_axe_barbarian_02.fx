#############################################################
## Costume Anim Entity - Left Blade - 02
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
		Bhvr	WEAPONS\Custom_Axe\Female_WepRScale.bhvr
		Anim	CustomWeapon_Axe_Barbarian_02
	End
End

#############################################################

End