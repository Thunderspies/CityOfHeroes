#########################################################
##	
##

FxInfo
LifeSpan  500

##################################

Condition
	On Time
	Time 0

	Event
		EName 	Prime
		Type	start
		At	root 
		
#		ChildFX	V_COV\EnemyPowers\Guards\F_Teleportation.fx
		Lifespan 60
	End
End

Condition
	On Time
	Time 250

	Event
		EName 	Prime
		Type	start
		At	root 
		
#		ChildFX	V_COV\EnemyPowers\Guards\F_Teleportation.fx
		Lifespan 60
	End
End