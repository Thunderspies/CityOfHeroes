#############################################################
## Costume Anim Entity - Katana - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	WepL
#		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScaleAndFlip.bhvr
#		Anim	CustomWeapon_BladeRight_Talsorian_Grip01
#	End
#	Event
#		Type	Local
#		At	WepL
#		Bhvr	WEAPONS\Custom_DualBlades\Female_WepLScaleAndFlip.bhvr
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
		At	WepL
                While   DUALHAND
                Flags   OneAtATime
		Anim	CustomWeapon_BladeLeft_Mecha_01
		BhvrOverride
			PositionOffset	0 0 0
			PyrRotate 180 180 0
		End
		ChildFX WEAPONS\Custom_Broadsword\Mecha_Sword01_CHILD.fx
	End
End

#############################################################

End