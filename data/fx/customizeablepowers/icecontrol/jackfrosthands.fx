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
		Part1	CustomizeablePowers\IceControl\snowburst.part
		Sound Frost 100 100 .8
		LifeSpan		45
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburst.part
		
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
		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan		45		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan		45
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan		47

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan		47

	End
	
End

Condition
	On	Time
	Time	37
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceControl\snowburstskatter2.part
		Part2	CustomizeablePowers\IceControl\FreezingTouchRing.part
		Sound Frost 100 100 .8
		LifeSpan		25
		
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburstskatter2.part
		Part2	CustomizeablePowers\IceControl\FreezingTouchRing.part
		Sound Frost 100 100 .8
		LifeSpan		25
		
	End

End

End				