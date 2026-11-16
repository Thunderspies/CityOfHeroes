#############################################################
## Costume Anim Entity - Axe - 01
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
		Bhvr	WEAPONS\Custom_Axe\Female_WepRScale.bhvr
		Anim	CustomWeapon_Axe_TalsorianBlade_01
	End

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Axe\Female_WepRScale.bhvr
		Anim	CustomWeapon_Axe_TalsorianShaft_01
	End
End

Condition
        On		Triggerbits
        Triggerbits	RIGHTHAND
        DoMany		1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Axe\Female_WepRScale.bhvr
		ChildFX WEAPONS\Custom_Axe\FX_Axe_Talsorian_01.fx
	End
End

#############################################################

End