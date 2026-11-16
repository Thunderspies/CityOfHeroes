#########################################################
##	Monkey Kick
FxInfo

LifeSpan	100

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Chest
End

Lifespan 100

##################################


Condition
	On	Time
	Time	5
		
	Event	
		ENAME	Jump
		Type	Local
		At	Chest
		part1	:MartialArtsShard.part
		part2	:EaglesClawChestGlow.part
		Sound EaglesClaw1 60 60 1.0
		LifeSpan 47
	
	End

End

Condition
	On	Time
	Time	35
		
	Event	
		ENAME	KickStreak
		Type	Local
		At	FootR
		part1	:MAKickStreak01.part
		part2	:MAMonkeyKickStreak01.part
		part3	:MAKickStar01.part
		LifeSpan 15
	
	End

End

Condition
	On	Time
	Time	35
		
	Event	
		ENAME	KickStreak2
		Type	Local
		At	FootR
		part1	:EaglesClawShard.part
		part2	:MonkeyKickSparkles.part
		LifeSpan 17
	
	End

End

Condition
	On	Time
	Time	48
		
	Event	
		ENAME	Ring1
		Type	Local
		At	Chest
		part1	:EaglesClawRing01.part
		LifeSpan 50
	
	End

End

Condition
	On	Time
	Time	42
		
	Event	
		ENAME	Foot
		Type	Local
		At	FootR
		part1	:MartialArtsStar01.part
		part2	:MartialArtsStar02.part
		part3	:MartialArtsStarGlow.part
		LifeSpan 1
	End

End

Condition
	On	Time
	Time	56
		
	Event	
		ENAME	FootFlash
		Type	Local
		At	FootR
		part1	:MartialArtsStarBig.part
		LifeSpan 20
	End

End

End						