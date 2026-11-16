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
#		Bhvr	WEAPONS\Custom_Katana\Female_WepLScale.bhvr
#		Anim	CustomWeapon_Katana_TalsorianGrip_01
#	End
#	Event
#		Type	Local
#		At	WepL
#		Bhvr	WEAPONS\Custom_Katana\Female_WepLScale.bhvr
#		Anim	CustomWeapon_Katana_TalsorianBlade_01
#	End
#End

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
		Anim	CustomWeapon_Katana_Mecha_02
		BhvrOverride
			PositionOffset	0 0 -0.05
			PyrRotate 180 180 0
		End
		ChildFX WEAPONS\Custom_Katana\Mecha_Katana02_CHILD.fx
	End
End

#############################################################

End