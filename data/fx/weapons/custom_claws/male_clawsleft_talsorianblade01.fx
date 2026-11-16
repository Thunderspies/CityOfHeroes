#############################################################
## Costume Anim Entity - Left Claws - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	WEAPONS\Custom_Claws\Male_LArmLScaleAndFlip.bhvr
		Anim	CustomWeapon_ClawsRight_TalsorianBlade01
	End
End

Condition
        On              Triggerbits
        Triggerbits     DUALHAND
        DoMany          1

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	LArmL
		Bhvr	WEAPONS\Custom_Claws\Male_LArmLScaleAndFlip.bhvr
                While   DUALHAND
                Flags   OneAtATime
		ChildFX WEAPONS\Custom_Claws\FX_ClawsRight_TalsorianBlade_01.fx
	End
End

#############################################################

End