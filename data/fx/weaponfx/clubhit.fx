#########################################################
##	template

FxInfo
Input  
	Inpname	Chest
End

Lifespan 30

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
		Sound Axe7 70.0 70.0 .75

	End

End

End

	

	