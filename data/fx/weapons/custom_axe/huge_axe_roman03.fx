#############################################################
## Costume Anim Entity - Axe - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Axe\Huge_WepRScale.bhvr
		Anim	CustomWeapon_Axe_Roman03
	End
End

#############################################################

End