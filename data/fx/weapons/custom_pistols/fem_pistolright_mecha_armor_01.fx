#############################################################
##  Costume Anim Entity - Right Pistol - Base
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
		Anim	CustomWeapon_PistolRight_Mecha_Armor_01
		BhvrOverride
			PositionOffset	0  	0     	0
			Scale		0.88    0.88    0.88
			PyrRotate 	0      	0     	0
		End
		Childfx	WEAPONS\Custom_Pistols\Pistol_Mecha_LaserSight.fx
	End
End

#############################################################

End