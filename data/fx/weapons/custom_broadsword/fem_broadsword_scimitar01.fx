#############################################################
## Costume Anim Entity - Broadsword 03
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
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_ArabianScimitar_01
	End
End

#############################################################

End