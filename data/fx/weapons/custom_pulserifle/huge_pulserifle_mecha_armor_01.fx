#############################################################
## Costume Anim Entity - Assault Rifle - Base
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
		Anim	CustomWeapon_PulseRifle_Mecha_Armor_01
#		BhvrOverride
#			PositionOffset	0.05  	-0     	-0.05
#			Scale		1.2    1.2     	1.2
#			PyrRotate 	0      3     	-1
	End
End

#############################################################

End