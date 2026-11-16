#########################################################
##	Thunder Kick
FxInfo

Lifespan 100

##################################

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	KickFlash
		Type	Local
		At	WepR
		part1	:MAKickStreak01.part
		part2	:MAKickStreak02.part
		part3	:MAKickStar01.part
		Sound MartialArtsPunchWindup 60 60 .66
		LifeSpan 30
	
	End

	Event	
		ENAME	KickFlash
		Type	Local
		At	WepL
		part1	:MAKickStreak01.part
		part2	:MAKickStreak02.part
		part3	:MAKickStar01.part

		LifeSpan 30
	
	End
End


						