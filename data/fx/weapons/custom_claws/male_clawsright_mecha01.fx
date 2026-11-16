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
#		At	WepR
#		#Bhvr	WEAPONS\Custom_Claws\Male_WepLScaleAndFlip.bhvr
#		Anim	CustomWeapon_ClawsRight_Mecha01
#		ChildFX :Mecha_ClawR01_CHILD.fx
#	End
#End

Condition
        On              Triggerbits
        Triggerbits     DUALHAND
        DoMany          1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	WepR
                While   DUALHAND
                Flags   OneAtATime
		Anim	CustomWeapon_ClawsRight_Mecha01
		ChildFX :Mecha_ClawR01_CHILD.fx
	End
End

#############################################################

End