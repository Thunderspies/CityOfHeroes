#########################################################
##	
##
FxInfo

Flags	InheritAlpha

##########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	LLEGL
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_LGollemLegLeft
	End

	Event
		Type	Local 
		At	LLEGR
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_LGollemLegRight
	End

###################################################################

	Event
		Type	Local 
		At	UlegR
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_UGollemLegRight
	
	End

	Event
		Type	Local 
		At	ULEGL
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_UGollemLegLeft
		
	End

##########################################################

	Event
		Type	Local 
		At	Hips 
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		
		geom	Coral_FX_GollemHips
		
	End

#################################################################

	Event
		Ename	Heada
		Type	Local 
		At	CHEST
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_HeadChest
		
	End

	Event
		Ename	eye1
		Type	Local 
		At	Heada
		altpiv	1
	End

	Event
		Ename	eye2
		Type	Local 
		At	Heada
		altpiv	2
	End

#################################################################

	Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_LGollemArmRight
		

	End

	Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_LGolemArmLeft
		
	End

#################################################################
	
	Event
		Type	Local 
		At	UARML
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_UGolemArmLeft
	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	behaviors/StoneArmorGollem.bhvr
		geom	Coral_UGolemArmRight
	End

End

###############################################################################
##########################################################

Condition
	On 	death
	
	Event
		Ename	FinalRocks
		Type	local
		At	hips
		Part1	powers/earthcontrol/RocksScatter2.part
		Part2	powers/earthcontrol/RocksScatter.part
		LifeSpan	19
		
	End
	
	

	Event
		Ename	FinalDust
		Type	local
		At	hips
		Part3	powers/earthcontrol/StoneArmordirtExplosion.part
		Part1	powers/earthcontrol/StoneArmordirtExplosion1.part
		LifeSpan	7
	End

End

End	