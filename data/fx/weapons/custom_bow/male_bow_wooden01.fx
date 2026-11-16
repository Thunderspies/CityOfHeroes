#############################################################
## Costume Anim Entity - Bow 01
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_Bow_01
	End
End

#############################################################

End