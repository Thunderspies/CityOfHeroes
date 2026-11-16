#############################################################
## Costume Anim Entity - Right Claws - 03
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmR
		Bhvr	WEAPONS\Custom_Claws\Female_LArmRScale.bhvr
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
		At	LArmR
                While   DUALHAND
                Flags   OneAtATime
		Bhvr	WEAPONS\Custom_Claws\Female_LArmRScale.bhvr
		ChildFX WEAPONS\Custom_Claws\FX_ClawsRight_TalsorianBlade_01.fx
	End
End

#############################################################

End