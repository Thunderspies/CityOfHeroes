###############################################################
## Costume Anim Entity - Right Blade - Fire Sword from Power FX
###############################################################

FxInfo

Flags InheritAlpha DontSuppress

###############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
		Anim	CustomWeapon_BladeRight_FireSword_01
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
		BhvrOverride
			PositionOffset	0 0 0.3
#			PyrRotate	90 0 0
		End
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScale.bhvr
		ChildFX WEAPONS\Custom_DualBlades\FX_DualBlades_FireSword01.fx
	End
End

###############################################################

End