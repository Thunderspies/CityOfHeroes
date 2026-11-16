#############################################################
## Costume Anim Entity - Mace - 06
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
		Anim	CustomWeapon_Mace_Tech01
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Anim	CustomWeapon_Mace_Tech01_Cube
	End
End

Condition
        On              Triggerbits
        Triggerbits     RIGHTHAND
        DoMany          1

	Event
		EName	TechMaceFX
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		ChildFX WEAPONS\Custom_Mace\FX_TechMace01\TechMace_HeadGlow01.fx
	End
End

#############################################################

End