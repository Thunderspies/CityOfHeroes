#########################################################
##	template

FxInfo
Input  
	Inpname	Chest
End

Lifespan 60

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Part1	:MartialArtsHit.part
		Part2	:MartialArtsHit1.part
		Part3	:MartialArtsHit2.part
		Part4	:MartialArtsHitRaysSmall.part
		Sound KnifeStab3 100.0 100.0 .6

	End

End

Condition
	On Time 
	Time 0
	
	Repeat	2
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On Time 
	Time 2
	
	Chance	.75
	Repeat	3
	RepeatJitter	3
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End


End

	
End

	

	