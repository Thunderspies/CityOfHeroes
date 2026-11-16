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
		Part	Powers\MartialArts\MartialArtsHit.part
		Part	Powers\MartialArts\MartialArtsHit1.part
		Part	Powers\MartialArts\MartialArtsHit2.part
		Part	Powers\MartialArts\MartialArtsHitRaysSmall.part
		Part	Powers\MartialArts\MAHitGlow01.part
		Sound reclusepunch 70.0 70.0 .8
	End

End


End
