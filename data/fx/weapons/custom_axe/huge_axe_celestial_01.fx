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
			#Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_Axe_Celestial_01
	End
End

#############################################################

End