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
		
		part	:NictusCoreEnergyPurple.part
		part	:NictusCoreEnergyTeal.part
		part	:NictusCore2.part
		
	End

	Event
		EName	Chest
		Type	Local
		At	Waist
		
		part	:NictusCore2.part
		Part	:NictusCoreSparks2.part
	End

	Event
		EName	Chest
		Type	Local
		At	Back
		
		part	:NictusCore2.part
		
		#Part	:NictusCoreSparks2.part
	End
	
	Event
		EName	Chest
		Type	Local
		At	COL_L
		part	:NictusCore2.part
		part	:NictusCoreEnergy.part
		part	:NictusCoreEnergy1.part
		part	:NictusCoreEnergyPurpleTop.part
		part	:NictusCoreEnergyTealTop.part
		
	End

	Event
		EName	Chest
		Type	Local
		At	COL_R
		part	:NictusCore2.part
		part	:NictusCoreEnergy.part
		part	:NictusCoreEnergy1.part
		part	:NictusCoreEnergyPurpleTop.part
		part	:NictusCoreEnergyTealTop.part
		
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part	:NictusCore1.part
		part	:NictusCoreEnergy2.part
		part	:NictusCoreEnergyTeal2.part
		part	:NictusEnergyPurple1.part
		part	:NictusEnergyTeal1.part
		magnet	UlegL
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part	:NictusCore1.part
		part	:NictusCoreEnergy2.part
		part	:NictusCoreEnergyTeal2.part
		part	:NictusEnergyPurple1.part
		part	:NictusEnergyTeal1.part
		
		magnet	UlegL
	End

	Event
		EName	Uleg
		Type	Local
		At	UlegL
		part	:NictusCoreHips.part
		
		magnet	waist
	End

	Event
		EName	URleg
		Type	Local
		At	UlegR
		part	:NictusCoreHips.part
				
		magnet	waist
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		part	:NictusCore.part
		part	:NictusEnergyPurple2.part
		part	:NictusEnergyTeal2.part
		
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		part	:NictusCore.part
		part	:NictusEnergyPurple2.part
		part	:NictusEnergyTeal2.part
		
	End

	Event
		EName	Back
		Type	Local
		At	Back
		part	:NictusCore3.part
		
	End

End


End				