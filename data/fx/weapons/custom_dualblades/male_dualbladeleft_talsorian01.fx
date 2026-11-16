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
		At	WepL
		Bhvr	behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_BladeRight_Talsorian_Grip01
	End
	Event
		Type	Local
		At	WepL
		Bhvr	behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_BladeRight_Talsorian_Blade01
	End
End

Condition
        On		Triggerbits
        Triggerbits	DUALHAND
        DoMany		1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepL
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	behaviors\Reverse_Wep.bhvr
		ChildFX WEAPONS\Custom_DualBlades\FX_DualBlades_Talsorian_01.fx
	End
End

#############################################################

End