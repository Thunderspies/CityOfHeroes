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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScaleAndFlip.bhvr
		Anim	CustomWeapon_Broadsword_VanguardKatana_Grip01
	End

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScaleAndFlip.bhvr
		Anim	CustomWeapon_Broadsword_VanguardKatana_Blade01
	End
End

Condition
        On              Triggerbits
        Triggerbits     RIGHTHAND
        DoMany          1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepR
                While   RIGHTHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScaleAndFlip.bhvr
		ChildFX WEAPONS\Custom_Katana\FX_Katana_Talsorian_01.fx
	End
End

#############################################################

End