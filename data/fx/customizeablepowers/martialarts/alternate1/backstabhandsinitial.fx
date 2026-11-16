#########################################################
##	Thunder Kick
FxInfo

Lifespan 100

##################################

Condition
	On	Time
	Time	15
		
	Event	
		ENAME	KickFlash
		Type	Local
		At	WepR
		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\MAKickStreak02.part
		part3	CustomizeablePowers\MartialArts\MAKickStar01.part
		Sound punchBB 60 60 .88
		LifeSpan 60
	
	End

	Event	
		ENAME	KickFlash
		Type	Local
		At	WepL
		part1	CustomizeablePowers\MartialArts\MAKickStreak01.part
		part2	CustomizeablePowers\MartialArts\MAKickStreak02.part
		part3	CustomizeablePowers\MartialArts\MAKickStar01.part

		LifeSpan 60
	
	End
End


						