#############################################################
## Costume Anim Entity - Mace - Base
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
		Bhvr	WEAPONS\Custom_Mace\Female_WepRScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_Mace_Base
	End
End

#############################################################

End