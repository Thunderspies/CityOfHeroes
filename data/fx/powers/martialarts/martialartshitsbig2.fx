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
		
		Part	:MartialArtsHitRaysLarge2.part

		Part	:MartialArtsHit.part
		Part	:MartialArtsHit1.part
		Part	:MartialArtsHit2.part
		Part	:MartialArtsHitRaysSmall.part
		Part	:MAHitGlow01.part
		Sound PP13 60 60 .86
	End

End


End
