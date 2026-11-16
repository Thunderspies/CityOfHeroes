#############################################################
## Costume Anim Entity - Broadsword 01
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
		Anim	CustomWeapon_BladeRight_Imperial_Dynasty_01
		BhvrOverride
		PositionOffset	-0.01 .02 0.1
		Scale		0.8125 0.8125 0.8125
	End
End

#############################################################

End