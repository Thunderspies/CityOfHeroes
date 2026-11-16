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
		EName	ShieldOffset
		Type	local
		At	LArmL
		Bhvr	:Huge_Shield_Energy1.bhvr
		Anim	CustomShield_Energy1_01
	End
End

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	EnergyShield
		Type	local
		At	ShieldOffset
                While   SHIELD
		Until	DEATH
                Flags   OneAtATime
		ChildFX	:Child_EnergyShield.fx
	End
End

#############################################################

End