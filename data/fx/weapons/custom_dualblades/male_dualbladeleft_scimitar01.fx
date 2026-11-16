#############################################################
## Costume Anim Entity - Left Blade - Base
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
		Bhvr	behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_BladeRight_Base
	End
End

#############################################################

End