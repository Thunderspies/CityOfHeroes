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
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_Ice_Sword
	End
End


#############################################################

End