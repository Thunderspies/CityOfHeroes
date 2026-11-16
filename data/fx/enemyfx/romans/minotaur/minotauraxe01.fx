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
		BhvrOverride
			Scale		1.6 1.6 1.6
		End
		Anim	CustomWeapon_Axe_Roman02
	End
End

#############################################################

End