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
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Katana_TalsorianGrip_01
	End
	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Katana_TalsorianBlade_01
	End
End

Condition
        On              Triggerbits
        Triggerbits     LEFTHAND
        DoMany          1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepL
                While   LEFTHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Katana\Huge_WepLScale.bhvr
		ChildFX WEAPONS\Custom_Katana\FX_Katana_Talsorian_01.fx
	End
End

#############################################################

End