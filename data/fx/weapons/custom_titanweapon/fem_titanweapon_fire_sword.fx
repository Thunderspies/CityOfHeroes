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
		Anim	CustomWeapon_TitanWeapon_Fire_Sword
		BhvrOverride
			Scale		0.85 0.85 0.85
			PositionOffset	0 0 -0.1
			PyrRotate	-3 3 0
		End
	End
End


#############################################################

End