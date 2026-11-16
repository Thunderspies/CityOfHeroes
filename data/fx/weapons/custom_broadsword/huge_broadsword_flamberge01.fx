#############################################################
## Costume Anim Entity - Broadsword 02
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
		Anim	CustomWeapon_Broadsword_02
	End
End

#############################################################

End