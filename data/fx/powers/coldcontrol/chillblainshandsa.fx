#########################################################
##	ChillingTouchHands
FxInfo

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End



##################################

LifeSpan 50

Condition
	On	Time
	Time	0
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Sound Frost 100 100 .8
		LifeSpan		45
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		
		LifeSpan		45

	End

End

Condition
	On	Time
	Time	5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:ColdArmMist.part
		LifeSpan		45		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	:ColdArmMist.part
		LifeSpan		45
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:FrostHands.part
		LifeSpan		47

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:FrostHands.part
		LifeSpan		47

	End
	
End

Condition
	On	Time
	Time	22
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburstskatter2.part
		Part2	:FreezingTouchRing2.part
		Sound Frost 100 100 .8
		LifeSpan		20
		
	End

End


End				