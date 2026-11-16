#########################################################
##	IceBolt
###########################################################################
FxInfo

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Head
End

LifeSpan	200

#####################################################################
Condition
	On	Time
	Time	8

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IceControl\snowburst.part
		Sound Coldblast2 100 100 .8
		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburst.part
		LifeSpan	70
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	CustomizeablePowers\IceControl\ColdArmMist.part

		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	CustomizeablePowers\IceControl\ColdArmMist.part

		LifeSpan	70

	End

End

Condition
	On	Time
	Time	28

	Event
		EName 	Prime
		Type	Local 
		At	Head

		Part1	CustomizeablePowers\IceControl\shiverIce.part
		LifeSpan	55	
	End
	
	Event
		EName 	Prime2
		Type	Local 
		At	Head
		
		Part3	CustomizeablePowers\IceControl\ShiverSnow.part
		Part3	CustomizeablePowers\IceControl\ShiverMist.part
		LifeSpan	37	
	End

End

End				