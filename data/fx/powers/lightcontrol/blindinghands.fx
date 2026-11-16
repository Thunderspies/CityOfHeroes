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
		Part1	:BlindingDust.part
		#Part2	:ColdArmMist.part

		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:BlindingDust.part
		#Part2	:ColdArmMist.part
	End

End

Condition
	On	Time
	Time	3.5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		#Part1	:snowburst.part
		Part2	:BlindingArmMist.part

		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		#Part1	:snowburst.part
		Part2	:BlindingArmMist.part
	End

End


End				