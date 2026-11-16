#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End

Lifespan 10


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
		Sound BigMetalhit 60 60 .96
		Sound BigMetalhit2 60 60 .9
		Sound BigMetalhit3 60 60 .76
	End

End


End
