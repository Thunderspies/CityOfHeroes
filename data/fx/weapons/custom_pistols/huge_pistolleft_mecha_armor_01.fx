#############################################################
##  Costume Anim Entity - Left Pistol - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_PistolLeft_Mecha_Armor_01
		BhvrOverride
#			PositionOffset	0.05  	-0     	-0.05
			Scale		1.4    	1.4   	1.4
#			PyrRotate 	0      3     	180
		End
		Childfx	WEAPONS\Custom_Pistols\Pistol_Mecha_LaserSight.fx
	End
End

#############################################################

End