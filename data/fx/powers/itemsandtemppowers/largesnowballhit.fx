#########################################################
##	ChillingTouchHands
FxInfo

LifeSpan		35

##################################

Condition
	On	Time
	Time	0
		
	Event	
		Ename   LeftHandMist
		Type	start
		At	root
		bhvr	:LargeSnowPile.bhvr
		Geom	SnowballSnowHit
		LifeSpan		35
		
	End
	
	Event	
		Ename   LeftHandMist
		Type	Local
		At	Chest
		Part1	:LargeColdArmMist.part
		Part2	:LargeFrostFalling.part
		Part1	:Largesnowburst.part
		Sound Frost 100 100 .8
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
		Sound Frost 100 100 .8
		LifeSpan		25
		
	End

End

End				