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
		Bhvr	WEAPONS\Custom_AssaultRifle\Female_WepRScale.bhvr
		Anim	CustomWeapon_AssaultRifle_Mecha_Armor_01
		BhvrOverride
			PositionOffset	0.05  	0     	0
			Scale		0.7    	0.7     0.7
			PyrRotate 	0     	-9     	-4

	End
End

#############################################################

End