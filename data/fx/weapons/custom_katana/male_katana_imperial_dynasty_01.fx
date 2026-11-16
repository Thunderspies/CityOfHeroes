#############################################################
## Costume Anim Entity - Katana - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Anim	CustomWeapon_Imperial_Dynasty_Katana_01
		Bhvr	Behaviors\Reverse_Wep.bhvr
	End
End

#############################################################

End