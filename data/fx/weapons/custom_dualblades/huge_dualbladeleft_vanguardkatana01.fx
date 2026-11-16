#############################################################
## Costume Anim Entity - Right Blade - 05
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
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepLScale.bhvr
		Anim	CustomWeapon_BladeLeft_VanguardKatana_Grip01
	End

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepLScale.bhvr
		Anim	CustomWeapon_BladeLeft_VanguardKatana_Blade01
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
		Bhvr	WEAPONS\Custom_DualBlades\Huge_WepLScale.bhvr
		ChildFX WEAPONS\Custom_Katana\FX_Katana_Talsorian_01.fx
	End
End

#############################################################

End