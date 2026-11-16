#############################################################
## Costume Anim Entity - Left Blade - Ice Sword from Power FX
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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_BladeRight_IceSword_01
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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScaleAndFlip.bhvr
		ChildFX WEAPONS\Custom_DualBlades\FX_DualBlades_IceSword01.fx
	End
End

#############################################################

End

