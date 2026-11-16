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
	repeat	10

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		ChildFx	V_COV/PhysicsEnabled/PoisonDripSmall.fx
		LifeSpan	10

	End
End

Condition
	On	Time
	Time	3
	repeat	10

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		ChildFx	V_COV/PhysicsEnabled/PoisonDripSmall.fx
		LifeSpan	10

	End
End

Condition
	

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDustBurstB.part
		Sound poisonarrow3 70 70 .78
		Sound poisonarrow4 70 70 .77
		LifeSpan	5

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDust2BurstB.part
		LifeSpan	10
	End
	
End


End			