#############################################################
## Costume Anim Entity - Broadsword - Base
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
		BhvrOverride
			Scale	0.875 0.875 0.875
		End
		Anim	CustomWeapon_Broadsword_Romulus_01
	End
End

Condition
        On		Triggerbits
        Triggerbits	RIGHTHAND
        DoMany		1

	Event
		EName	NictusEnergy
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		BhvrOverride
			Scale	0.875 0.875 0.875
		End
		ChildFX WEAPONS\Custom_Broadsword\RomulusNictus\NictusBroadsword_01.fx
	End
End

#############################################################

End