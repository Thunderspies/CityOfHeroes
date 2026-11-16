#############################################################
## Costume Anim Entity - Broadsword - Base
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
		BhvrOverride
			Scale	0.875 0.875 0.875
		End
		Anim	CustomWeapon_Broadsword_Romulus_01
	End
End

#############################################################

End