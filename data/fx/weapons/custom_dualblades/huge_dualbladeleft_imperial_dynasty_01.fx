#############################################################
## Costume Anim Entity - Left Blade - 02
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
		Anim	CustomWeapon_BladeLeft_Imperial_Dynasty_01
		BhvrOverride
		PositionOffset	0.0 -.01 0.1
	End
End

#############################################################

End