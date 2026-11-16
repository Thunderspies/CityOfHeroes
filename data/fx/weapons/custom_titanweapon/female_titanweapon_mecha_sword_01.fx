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
#		Anim	CustomWeapon_TitanWeapon_Mecha_Sword_01
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
		Bhvr	WEAPONS\Custom_TitanWeapon\Female_WepLScale.bhvr
		Anim	CustomWeapon_TitanWeapon_Mecha_Sword_01
		BhvrOverride
			Scale		0.85 0.85 0.87
			PositionOffset	-0.05 0 -0.15
			#PyrRotate	-3 3 0
		End
		ChildFX :Mecha_Sword01_CHILD.fx
	End
End

#############################################################

End