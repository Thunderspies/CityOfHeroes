#########################################################
##	
##
FxInfo

Lifespan	120

##########################################################

#Condition
#
#	Event
#		Ename	Snakes
#		Type	posit 
#		At	chest
#		ChildFx	V_COV/EnemyPowers\SnakeMen\SnakeImobilizeBelt.fx
#		LifeSpan	10
#
#	End
#End
#
Condition
	repeat	28

	Event
		Ename	Snakes
		Type	posit 
		At	T_chest
		ChildFx	V_COV/PhysicsEnabled/PoisonDripSmallAOE.fx
		LifeSpan	50

	End
End

Condition
	

	Event
		Ename	Snakes
		Type	posit 
		At	T_chest
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDustBurstAOE.part
		LifeSpan	5

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	T_chest
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDust2BurstAOE.part
		LifeSpan	10
	End
	
End


End			