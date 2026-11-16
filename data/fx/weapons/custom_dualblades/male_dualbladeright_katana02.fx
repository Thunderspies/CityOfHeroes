#############################################################
## Costume Anim Entity - Right Blade - 04
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
		Bhvr	behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_BladeLeft_Katana_02
	End
End

#############################################################

End