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
		
		Part	CustomizeablePowers\MartialArts\MartialArtsHitRaysLarge2.part

		Part	CustomizeablePowers\MartialArts\MartialArtsHit.part
		Part	CustomizeablePowers\MartialArts\MartialArtsHit1.part
		Part	CustomizeablePowers\MartialArts\MartialArtsHit2.part
		Part	CustomizeablePowers\MartialArts\MartialArtsHitRaysSmall.part
		Part	CustomizeablePowers\MartialArts\MAHitGlow01.part
		Sound PP13 60 60 .86
	End

End


End
