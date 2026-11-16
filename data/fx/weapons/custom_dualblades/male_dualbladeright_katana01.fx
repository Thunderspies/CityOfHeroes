#############################################################
## Costume Anim Entity - Right Blade - 05
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
		Anim	CustomWeapon_BladeLeft_Katana_01
	End
End

#############################################################

End