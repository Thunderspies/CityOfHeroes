
#########################################################################################
########	Notes: Recluse Mele attack with streaking from arms
########                           
#########################################################################################

FxInfo
	
LifeSpan	90

##################################################################################
###########################  Streaks for left arm ######################  01  ####
##################################################################################

Condition
	On	Time
	Time	18
	
	Event
		EName	Right01
		Type	Local
		At	Chest
		
		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_ChildStreak_RightArmMele01.fx
	End
End

Condition
	On	Time
	Time	22
	
	Event
		EName	Left
		Type	Local
		At	Chest
		
		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_ChildStreak_LeftArmMele.fx
	End
End

Condition
	On	Time
	Time	34
	
	Event
		EName	Right02
		Type	Local
		At	Chest
		
		ChildFX V_COV\EnemyPowers\ArchVillains\Lord_Recluse\F_ChildStreak_RightArmMele01.fx
	End
End


End