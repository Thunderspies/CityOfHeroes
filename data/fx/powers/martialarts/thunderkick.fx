#########################################################
##	Thunder Kick
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
	Time	9
		
	Event	
		ENAME	KickFlash
		Type	Local
		At	FootR
		part1	:MAKickStreak01.part
		part2	:MAKickStreak02.part
		part3	:MAKickStar01.part
		LifeSpan 10
	
	End

End

Condition
	On	Time
	Time	5
		
	Event	
		Type	Local
		At	FootR
		Sound whoosh13 60 60 .88
		LifeSpan 10
	
	End

End



						