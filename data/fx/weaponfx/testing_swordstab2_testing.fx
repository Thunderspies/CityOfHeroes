#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


Condition
	On 	Time
	Time 	15

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Part1	:MartialArtsHit.part
		Part2	:MartialArtsHit1.part
		Part3	:MartialArtsHit2.part
		Part4	:MartialArtsHitRaysSmall.part
		Sound SwordStab1 100.0 100.0 .9

	End

End


Condition
	On	Time
	Time	7

	Event	
	Type 	Local
	At	chest
	Sound SwordStab2 100.0 100.0 .8

	End

End

End

	

	