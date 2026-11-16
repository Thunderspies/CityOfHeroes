#############################################################
## Costume Anim Entity - Pulse Rifle - 01
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
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		BhvrOverride
			Scale		0.82 0.82 0.82
			PositionOffset	-0.1 0 -0.2
			PyrRotate	0 0 0
		End
		Anim	CustomWeapon_TitanWeapon_Garage_Brand
	End
End

#############################################################

End