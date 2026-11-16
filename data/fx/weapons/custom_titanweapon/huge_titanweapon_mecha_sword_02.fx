#############################################################
## Costume Anim Entity - Pulse Rifle - 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress InheritAnimScale

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	WepL
#		#Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
#		Anim	CustomWeapon_TitanWeapon_Mecha_Sword_02
#		BhvrOverride
#			#Scale		0.85 0.85 0.85
#			PositionOffset	0 0 0
#			#PyrRotate	-3 3 0
#		End
#	End
#End

Condition
        On		Triggerbits
        Triggerbits	2HLARGE #WEAPON works too
        DoMany		1

	Event
		EName	Mecha
		Type	Local
		At	WepL
                While   2HLARGE #WEAPON
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_TitanWeapon\Huge_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_Mecha_Sword_02
		BhvrOverride
			PositionOffset	0 0 -0.2
		End
		ChildFX :Mecha_Sword02_CHILD.fx
	End
End
#############################################################

End