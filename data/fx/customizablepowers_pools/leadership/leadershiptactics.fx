#########################################################
##	Leadership Defense
FxInfo


Input  
	InpName	Hips
End

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	Hair
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Head
End

#Lifespan 150

##################################

Condition
	On	Time
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	Head
		part1	:LTacticsStar01.part
		pmagnet Chest
		Sound fanfare1 70 70 .8
		Lifespan 70
		
	End

End

Condition
	On	Time
	Time	12
	
	Event
		EName	Ring
		Type	Local
		At	Head
		part1	:LTacticsGlowLrg01.part
		part2	:LTacticsGlowSml01.part
		pmagnet Chest
		Sound heal1 70 70 .66
		Lifespan 150
		
	End

End

Condition
	On	Time
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	Head
		part1	:LTacticsRays01.part
		pmagnet Chest
		Lifespan 70
		
	End

End

End