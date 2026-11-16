#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		part2	:PeaceCoreEnergy.part

		part2	:PeaceCore2.part
		
	End

	Event
		EName	Chest
		Type	Local
		At	Waist
		
		part2	:PeaceCore2.part
		
		Part4	:PeaceCoreSparks2.part
	End
	
	Event
		EName	Chest
		Type	Local
		At	Col_L
		part1	:PeaceCore2.part
		part1	:PeaceCoreEnergyHead.part
	End

	Event
		EName	Chest
		Type	Local
		At	Col_R
		part1	:PeaceCore2.part
		part1	:PeaceCoreEnergyHead.part
		
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy2.part
		magnet	UlegL
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy3.part
		magnet	UlegL
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		part1	:PeaceCore.part
		part2	:PeaceCoreEnergy3.part
		
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		part1	:PeaceCore.part
		part2	:PeaceCoreEnergy2.part
		
	End

	Event
		EName	Back
		Type	Local
		At	Back
		part1	:PeaceCore3.part
		
	End

End


End				