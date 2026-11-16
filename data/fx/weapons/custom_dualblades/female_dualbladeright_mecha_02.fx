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
		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScale.bhvr
		Anim	CustomWeapon_Bladeright_Mecha_02
		BhvrOverride
			PositionOffset	0.01 0.02 -0.05
			#Scale	0.85 0.85 0.85
		End
		ChildFX WEAPONS\Custom_Broadsword\Mecha_Sword02_CHILD.fx
	End
End

#############################################################

End