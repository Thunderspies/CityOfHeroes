#########################################################
##	ChillingTouchHands
FxInfo


##################################

LifeSpan 50


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound Snowballhit 90 90 1.0
	End
End


Condition
	On	Time
	Time	0
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Chest
		Part1	:snowburst.part
		LifeSpan		30
		
	End
	
End

Condition
	On	Time
	Time	5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Chest
		Part1	:ColdArmMist.part
		Part2	:FrostFalling.part
		LifeSpan		35		
	End
		
	Event	
		Ename   LeftHandMist
		Type	Local
		At	Chest
		
		Part2	:FrostHands.part
		Part1	:snowburstskatter2.part
		LifeSpan		30

	End
	
End

Condition
	On	Time
	Time	10
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	Chest
		Part2	:FreezingTouchRing.part
		Sound Frost 90 90 .37
		LifeSpan		25
		
	End

End

End				