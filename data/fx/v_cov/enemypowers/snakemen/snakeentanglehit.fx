#########################################################
##	
##
FxInfo

Lifespan	120

##########################################################

###Condition
####	repeat	8
###
###	Event
###		Ename	Snakes
###		Type	posit 
###		At	Root
###		ChildFx	V_COV/EnemyPowers\SnakeMen\SnakeEntangle.fx
####		LifeSpan	10
###
###	End
###End
###
Condition
	repeat	8

	Event
		Ename	Snakes
		Type	posit 
		At	Root
		ChildFx	V_COV/PhysicsEnabled/PoisonDrip.fx
		LifeSpan	10

	End
End

Condition
	
	Event
		Ename	Snakes
		Type	posit 
		At	Root
		Bhvr	:SnakeFade.bhvr
#		Part	:SnakeDust2Burst.part
		Part	:SnakeDustBurst.part
		LifeSpan	5

	End
	
	Event
		Ename	Snakes
		Type	posit 
		At	Root
		Bhvr	:SnakeFade.bhvr
		Part	:SnakeDust2Burst.part
#		Part	:SnakeDustBurst.part
		LifeSpan	10
	End
	
End


End			