#############################################################
## Costume Anim Entity - Broadsword 08
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
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_08
	End
End

#############################################################

End