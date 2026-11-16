#############################################################
## Costume Anim Entity - Katana - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_Katana_Base
	End
End

#############################################################

End