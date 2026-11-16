#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

#LifeSpan	120

#################################################################
##Hands
#################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Sound eleccage_loop 70 70 .2
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\Red_ElectricityGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\Red_ElectricityGlowStar.part
		Part3	CustomizeablePowers\ElectricalMelee\Red_ElectricityArch4a.part
		Part4	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchSmalla.part
		Sound elecballhit2 50 50 .33
	End

End

Condition
	On	Time
	Time	0

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing2.part
		Part1	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing2.part

	End

End

Condition
	On	Time
	Time	3

	Event
		EName	continuingArchs
		Type	Local
		At	Chest

		Part1	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing2.part
		Part1	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\Red_ElectricityArchContinuing2.part
		#Sound	eleccage_loop 70 60 .1
	End
End

	Condition
	On 	Time
	Time 	21
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End



End
