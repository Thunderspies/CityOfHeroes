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
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		BhvrOverride
			Scale			1.125 1.375 1.125
		End
		Anim	CustomWeapon_Broadsword_Valkyrie_01
	End
End

#############################################################

End