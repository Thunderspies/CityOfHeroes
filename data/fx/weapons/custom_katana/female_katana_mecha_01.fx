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
		Bhvr	WEAPONS\Custom_Katana\Female_WepLScale.bhvr
		Anim	CustomWeapon_Katana_Mecha_01
		BhvrOverride
			PositionOffset	0 0 -0.1
			PyrRotate 180 180 0
			Scale 0 0 0.9
		End
		ChildFX WEAPONS\Custom_Katana\Mecha_Katana01_CHILD.fx
	End
End

#############################################################

End