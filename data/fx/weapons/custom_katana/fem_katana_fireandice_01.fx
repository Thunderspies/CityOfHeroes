#############################################################
## Costume Anim Entity - Katana - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		BhvrOverride
			Scale		0.75 0.75 0.75
			PYRRotate	0.0 0.0 180.0
		End
		Anim	CustomWeapon_Katana_FireAndIce_01
	End
End

#############################################################

End