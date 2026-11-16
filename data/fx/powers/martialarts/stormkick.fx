#########################################################
##	Storm Kick
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
##################################

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	:MartialArtsShard.part
		pmagnet	Chest
		Lifespan 29
		Sound Stormkick4 60 60 1.0
	End

End

Condition
	On	Time
	Time	3
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MartialArtsChestGlow.part
		
	
	End

End

Condition
	On	Time
	Time	8
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MartialArtsRing.part
		
	
	End

End

Condition
	On	Time
	Time	4
		
	Event	
		ENAME	FootStars
		Type	Local
		At	FootR
		part1	:MartialArtsCraneStar01.part
		part2	:MartialArtsCraneStar02.part
		part3	:MartialArtsCraneStarGlow.part

	
	End

End

Condition
	On	Time
	Time	7
		
	Event	
		ENAME	FootFlash
		Type	Local
		At	FootR
		part1	:MartialArtsStarBig.part
		Lifespan 32
	End

End

Condition
	On	Time
	Time	7
		
	Event	
		ENAME	FootBlur
		Type	Local
		At	FootR
		part1	:MAKickStreak02.part
		Lifespan 5
	End

End

Condition
	On	Time
	Time	11
		
	Event	
		ENAME	FootBlur2
		Type	Local
		At	FootR
		part1	:MAKickStreak02.part
		Lifespan 5
	End

End
End				#########################################################
##	Storm Kick
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
##################################

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	:MartialArtsShard.part
		pmagnet	Chest
		Lifespan 29
		Sound Stormkick4 60 60 1.0
	End

End

Condition
	On	Time
	Time	3
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MartialArtsChestGlow.part
		
	
	End

End

Condition
	On	Time
	Time	8
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		part1	:MartialArtsRing.part
		
	
	End

End

Condition
	On	Time
	Time	4
		
	Event	
		ENAME	FootStars
		Type	Local
		At	FootR
		part1	:MartialArtsCraneStar01.part
		part2	:MartialArtsCraneStar02.part
		part3	:MartialArtsCraneStarGlow.part

	
	End

End

Condition
	On	Time
	Time	7
		
	Event	
		ENAME	FootFlash
		Type	Local
		At	FootR
		part1	:MartialArtsStarBig.part
		Lifespan 32
	End

End

Condition
	On	Time
	Time	7
		
	Event	
		ENAME	FootBlur
		Type	Local
		At	FootR
		part1	:MAKickStreak02.part
		Lifespan 5
	End

End

Condition
	On	Time
	Time	11
		
	Event	
		ENAME	FootBlur2
		Type	Local
		At	FootR
		part1	:MAKickStreak02.part
		Lifespan 5
	End

End
End				