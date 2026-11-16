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
		Part	:MartialArtsHit.part
		Part	:MartialArtsHit1.part
		Part	:MartialArtsHit2.part
		Part	:MartialArtsHitRaysSmall.part
		Part	:MAHitGlow01.part
		Sound makick 60 60 1.0
	End

End


End
