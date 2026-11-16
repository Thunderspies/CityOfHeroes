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
		At	WepR
		Bhvr	behaviors\Reverse_Wep.bhvr
		Anim	CustomWeapon_BladeLeft_VanguardKatana_Grip01
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors\Reverse_Wep.bhvr
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
		At	WepR
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	behaviors\Reverse_Wep.bhvr
		ChildFX WEAPONS\Custom_Katana\FX_Katana_Talsorian_01.fx
	End
End

#############################################################

End