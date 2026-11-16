#########################################################
##	
##
FxInfo

##########################################################


Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Origin
		Sound snakehold2_loop 70 70 .5
	End
End


################################################################################


#
#Condition
#	On 	Cycle
#	Time	3
#	Chance	.1
#
#	Event
#		Ename	Snakes
#		Type	posit 
#		At	hips	#chest
#		Bhvr	:SnakeSpinBelt.bhvr
#		geom	SnakeBelt01
#		geom	SnakeBelt02
#		LifeSpan	80
#	End
#End
#
#
#Condition
#	On 	Cycle
#	Time	3
#	Chance	.1
#
#	Event
#		Ename	Snakes
#		Type	posit 
#		At	hips
#		Bhvr	:SnakeSpinBeltCounter.bhvr
#		geom	SnakeBelt03
#		geom	SnakeBelt04
#		LifeSpan	80
#
#	End
#	
#End
#

Condition
	On 	Cycle
	Time	8
	Chance	.1

	Event
		Ename	Snakes
		Type	posit 
		At	hips	#chest
		Bhvr	:SnakeSpinBelt.bhvr
		geom	SnakeBelt01
		LifeSpan	80
		

	End
End


Condition
	On 	Cycle
	Time	8
	Chance	.1
	
	Event
		Ename	Snakes
		Type	posit 
		At	hips	#chest
		Bhvr	:SnakeSpinBelt.bhvr
		geom	SnakeBelt02
		LifeSpan	80
		

	End
End


Condition
	On 	Cycle
	Time	8
	Chance	.1
	
	Event
		Ename	Snakes
		Type	posit 
		At	hips
		Bhvr	:SnakeSpinBeltCounter.bhvr
		geom	SnakeBelt03
		LifeSpan	80
		

	End
End


Condition
	On 	Cycle
	Time	8
	Chance	.1
	
	Event
		Ename	Snakes
		Type	posit 
		At	hips
		Bhvr	:SnakeSpinBeltCounter.bhvr
		geom	SnakeBelt04
		LifeSpan	80
		

	End

End
#############################################
##Chest
###########################

#Condition
#	On 	Cycle
#	Time	3
#	Chance	.1
#
#	Event
#		Ename	Snakes
#		Type	posit 
#		At	chest
#		Bhvr	:SnakeSpinBelt.bhvr
#		geom	SnakeBelt01
#		geom	SnakeBelt02
#		LifeSpan	80
#	End
#End
#
#
#Condition
#	On 	Cycle
#	Time	3
#	Chance	.1
#
#	Event
#		Ename	Snakes
#		Type	posit 
#		At	chest
#		Bhvr	:SnakeSpinBeltCounter.bhvr
#		geom	SnakeBelt03
#		geom	SnakeBelt04
#		LifeSpan	80
#
#	End
#	
#End
#
#End


Condition
	On 	Cycle
	Time	8
	Chance	.1

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeSpinBelt.bhvr
		geom	SnakeBelt01
		
		LifeSpan	80
		

	End
	
End


Condition
	On 	Cycle
	Time	8
	Chance	.1

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeSpinBelt.bhvr
		geom	SnakeBelt02
		
		LifeSpan	80
		

	End

	
End


Condition
	On 	Cycle
	Time	8
	Chance	.1
	
	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeSpinBeltCounter.bhvr
		geom	SnakeBelt03
		
		LifeSpan	80
		

	End
	
End


Condition
	On 	Cycle
	Time	8
	Chance	.1

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		Bhvr	:SnakeSpinBeltCounter.bhvr
		geom	SnakeBelt04
		
		LifeSpan	80

	End

End


End			