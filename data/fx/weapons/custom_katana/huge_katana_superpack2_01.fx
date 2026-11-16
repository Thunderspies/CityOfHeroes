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
		Anim	CustomWeapon_Katana_SuperPack2_01
		BhvrOverride
			Scale		0.8 0.8 0.8
//			TintGeom	1
		End
	End
End

#############################################################

End