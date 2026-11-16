#########################################################
##	ChillingTouchHands
FxInfo

Input  
	InpName	WepL
End

Input  
	InpName	WepR
End

LifeSpan	32

##################################



Condition
	On	Time
	Time	0
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		
		LifeSpan		30
		Sound chillingtouch2 80 80 .8
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		
		LifeSpan		30

	End

End

Condition
	On	Time
	Time	3.5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:ColdArmMist.part
		LifeSpan		30		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	:ColdArmMist.part
		LifeSpan		30
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:FrostHands.part
		LifeSpan		32

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:FrostHands.part
		LifeSpan		32

	End
	
End

Condition
	On	Time
	Time	18
		

	Event	
		Ename   LeftHandMist
		Type	start
		At	WepR
		Part1	:CrystalSnowBurst2.part
		Part2	:FreezingTouchRing.part
		LifeSpan		30		
	End

End

End				