#############################################################
## Costume Anim Entity - Katana - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	WepR
#		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
#		Anim	CustomWeapon_BladeRight_Talsorian_Grip01
#	End
#	Event
#		Type	Local
#		At	WepR
#		Bhvr	WEAPONS\Custom_DualBlades\Female_WepRScale.bhvr
#		Anim	CustomWeapon_BladeRight_Talsorian_Blade01
#	End
#End

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
		Anim	CustomWeapon_Bladeright_Mecha_01
		BhvrOverride
			PositionOffset	0 0 -0.08
		End
		ChildFX WEAPONS\Custom_Broadsword\Mecha_Sword01_CHILD.fx
	End
End

#############################################################

End