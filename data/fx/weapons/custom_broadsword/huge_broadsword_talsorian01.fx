#############################################################
## Costume Anim Entity - Left Blade - 02
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
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_Talsorian_Grip01
	End

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_Talsorian_Blade01
	End
End

Condition
        On		Triggerbits
        Triggerbits	RIGHTHAND
        DoMany		1

	Event
		EName	TalsorianSword_Glow
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		ChildFX WEAPONS\Custom_Broadsword\FX_Broadsword_Talsorian_01.fx
	End
End

#############################################################

End