#############################################################
## Costume Anim Entity - Left Blade - 02
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
#		At	WepR
#		Anim	CustomWeapon_Mecha_Sword_01.txt
#	End
#End

Condition
        On		Triggerbits
        Triggerbits	RIGHTHAND
        DoMany		1

	Event
		EName	MechaSword_Glow
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		Anim	CustomWeapon_Mecha_Sword_01
		BhvrOverride
			PositionOffset	0.01 0.02 -0.1
		End
		ChildFX :Mecha_Sword01_CHILD.fx
	End
End

#############################################################

End