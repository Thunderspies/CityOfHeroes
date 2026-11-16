#########################################################
##	template

FxInfo

Lifespan	44

Condition
	On 	Time
	Time 	5

	Event
		EName 	arrow
		Type	Local
		At	chest
		
		Part	:AcidFlashHit.part
		Part	:AcidHitFlashDark.part

		Lifespan	17

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	Local
		At	chest


		Part	:AcidHit.part
		Part	:AcidHitDark.part
		Part	:AcidBubblesHit.part
		Part	:AcidBubblesdarkHit.part
		Part	:AcidBubbleHit.part

		Lifespan	44

	End
	
End

Condition
	repeat	10

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		ChildFx	V_COV/PhysicsEnabled/PoisonDripDarkGreenSmall.fx
#		LifeSpan	10

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
		ChildFx	V_COV/PhysicsEnabled/PoisonDripDarkGreenSmall.fx
#		LifeSpan	10

	End
End

Condition
	On	Time
	Time	6
	repeat	6

	Event
		Ename	Snakes
		Type	posit 
		At	chest
		ChildFx	V_COV/PhysicsEnabled/PoisonDripDarkGreenSmall.fx
#		LifeSpan	10

	End
End

End

		