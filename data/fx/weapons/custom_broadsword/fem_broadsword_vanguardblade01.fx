#############################################################
## Costume Anim Entity - Katana - Base
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
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_VanguardBlade_Grip01
	End

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		Anim	CustomWeapon_Broadsword_VanguardBlade_Blade01
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
		Bhvr	WEAPONS\Custom_Broadsword\Female_WepRScale.bhvr
		ChildFX WEAPONS\Custom_DualBlades\FX_DualBlades_Talsorian_01.fx
	End
End

#############################################################

End