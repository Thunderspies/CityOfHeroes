#########################################################
##	ChillingTouchHands
FxInfo

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End


LifeSpan		110

##################################



Condition
	On	Time
	Time	5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		
		LifeSpan		95
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		
		LifeSpan		95

	End

End

Condition
	On	Time
	Time	8.5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:ColdArmMist.part
		LifeSpan		95		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	:ColdArmMist.part
		LifeSpan		95
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:FrostHands.part
		LifeSpan		97

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:FrostHands.part
		LifeSpan		97

	End
	
End


End				