#############################################################
## Costume Anim Entity - Left Claws - Base
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
#		Bhvr	WEAPONS\Custom_Claws\Male_WepLScaleAndFlip.bhvr
#		Anim	CustomWeapon_ClawsRight_Mecha01
#		ChildFX :Mecha_ClawL01_CHILD.fx
#	End
#End

Condition
        On              Triggerbits
        Triggerbits     DUALHAND
        DoMany          1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepL
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Claws\Female_WepLScaleAndFlip.bhvr
		Anim	CustomWeapon_ClawsRight_Mecha01
	End

	Event
		EName	TalsorianBlades_Glow2
		Type	Local
		At	WepL
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Claws\Female_WepLScaleAndFlip.bhvr
		BhvrOverride
				#PositionOffset 0 0.015 0.00
				PyrRotate 180 180 0
				#Scale 5 5 5
		End
		ChildFX :Mecha_ClawL01_CHILD.fx
	End
End

#############################################################

End