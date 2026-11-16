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
		Bhvr	:Male_Shield_Electrical1.bhvr
	End
End

Condition
        On              Triggerbits
        Triggerbits     SHIELD
        DoMany          1

	Event
		EName	ElectricShield1
		Type	local
		At	ShieldOffset
                While   SHIELD
		Until	DEATH
                Flags   OneAtATime
		Part1	:Electricity_Bolt1.part
		Part1	:Electricity_Bolt2.part
		Part3	:Electricity_Ring1.part
	End

	Event
		EName	ElectricShield2
		Type	local
		At	ShieldOffset
                While   SHIELD
		Until	DEATH
                Flags   OneAtATime
		Part1	:Electricity_Control1.part
		Part2	:Electricity_Control2.part
		Part3	:Electricity_SmallFlashes.part
		Part1	:Electricity_BigFlashes.part
	End
End

#############################################################

#Condition
#        On              Triggerbits
#        Triggerbits     DEATH
#
#	Event
#		Type	Destroy
#		EName	ElectricShield1
#	End
#	Event
#		Type	Destroy
#		EName	ElectricShield2
#	End
#End

#############################################################

End