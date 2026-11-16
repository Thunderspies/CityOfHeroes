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
		Sound BatonHit3 40.0 40.0 .7
		Sound BatonHit4 40.0 40.0 .7
		Sound BatonHit5 40.0 40.0 .7
		Sound BatonHit6 40.0 40.0 .5

	End

End

End

	

	