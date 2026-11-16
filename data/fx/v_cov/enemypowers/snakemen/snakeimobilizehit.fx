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
	repeat	8

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
		Part	:SnakeDustBurst.part
		Sound macehit2 70 70 .78
		LifeSpan	5

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDust2Burst.part
		LifeSpan	10
	End
	
End


End			