##############################################################
## Costume Anim Entity - Right Blade - Ice Sword from Power FX
##############################################################

FxInfo

Flags InheritAlpha DontSuppress

##############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
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
		At	WepR
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
		ChildFX WEAPONS\Custom_DualBlades\FX_DualBlades_IceSword01.fx
	End
End

#############################################################

End