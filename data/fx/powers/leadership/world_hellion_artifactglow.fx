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
	InpName	Eyes
End

#Lifespan 150

##################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	Eyes
		part1	:LTContinuingGlowLrg01.part
		part2	:LTContinuingRays01.part
		pmagnet Head
		Sound hellionglow_loop 40 40 1.0
		#Lifespan 70
		
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	Eyes
		#part1	:LTContinuingStarLrg01.part
		pmagnet Head
		
		#Lifespan 55
		
	End

End

Condition

	Event
		EName	Ring
		Type	Local
		At	Eyes
		#part1	:LTContinuingStarLrg01.part
		pmagnet Head
		
		#Lifespan 55
		
	End

End

Condition

	
	Event
		EName	Ring
		Type	Local
		At	Eyes
		part1	:LTContinuingGlowLrg01.part
		part2	:LTContinuingRays01.part
		pmagnet Head
		
		#Lifespan 70
		
	End

End


End