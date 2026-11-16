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
		PositionOffset	0.1 .02 0.1
		Scale		0.8125 0.8125 0.8125
		PyrRotate		0 -05 190
	End
End

#############################################################

End