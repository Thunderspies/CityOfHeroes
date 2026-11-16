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
		Bhvr	WEAPONS\Custom_AssaultRifle\Male_WepRScale.bhvr
		Anim	CustomWeapon_AssaultRifle_Mecha_Armor_01
#		BhvrOverride
#			PositionOffset	0  	0     	0
#			Scale		0    	0     	0
#			PyrRotate 	0      	0     	0

	End
End

#############################################################

End