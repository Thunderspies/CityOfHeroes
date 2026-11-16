#############################################################
## Costume Anim Entity - Shield - Roman 1
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:Female_Shields_Round1.bhvr
		Anim	CustomShield_Round1_RomulusNictus01
	End
End

Condition
        On		Triggerbits
        Triggerbits	RIGHTHAND
        DoMany		1

	Event
		EName	NictusEnergy
		Type	Local
		At	LArmL
                While   RIGHTHAND
                Flags   OneAtATime
		Bhvr	:Female_Shields_Round1.bhvr
		ChildFX WEAPONS\CustomShields\RomulusNictus\NictusShield_01.fx
	End
End

#############################################################

End