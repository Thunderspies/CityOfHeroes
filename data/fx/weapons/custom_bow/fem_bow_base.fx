#############################################################
## Costume Anim Entity - Bow Base
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
		Bhvr	WEAPONS\Custom_Bow\Female_WepLScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		Anim	CustomWeapon_Longbow_Base
	End
End

#############################################################

End