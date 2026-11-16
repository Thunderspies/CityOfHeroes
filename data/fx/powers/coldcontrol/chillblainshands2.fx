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



Condition
	On	Time
	Time	0
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Sound Chillingtouch3 100 100 .6
		LifeSpan		65
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		
		LifeSpan		65

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
		LifeSpan		65		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	:ColdArmMist.part
		LifeSpan		65
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:FrostHands.part
		LifeSpan		67

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:FrostHands.part
		LifeSpan		67

	End
	
End


End				