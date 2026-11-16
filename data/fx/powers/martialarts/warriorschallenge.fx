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
	Time	18
		
	Event	
		ENAME	LightShards
		Type	Local
		At	Chest
		part1	:MALightShardFaint.part
		pmagnet	Chest
		Lifespan 18
	End

End

Condition
	On	Time
	Time	15
		
	Event	
		ENAME	Glow
		Type	Local
		At	Chest
		part1	:MAWarrChalChestGlow01.part
		
	
	End

End

Condition
	On	Time
	Time	43
		
	Event	
		ENAME	Ring
		Type	Local
		At	Chest
		#part1	:MAWarrChalRing01.part
		
	
	End

End

Condition
	On	Time
	Time	41
		
	Event	
		ENAME	Rays
		Type	Local
		At	Chest
		#part1	:MartialArtsStarBig.part
		
	
	End

End


End				