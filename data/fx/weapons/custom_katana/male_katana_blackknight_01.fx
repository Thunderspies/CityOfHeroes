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
		Anim	CustomWeapon_Katana_BlackKnight_01
		BhvrOverride
			Scale		0.6 0.6 0.6
//			TintGeom	1
		End
	End
End

#############################################################

End