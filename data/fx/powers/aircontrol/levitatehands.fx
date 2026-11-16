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
	Time	3.5
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part1	Powers/ColdControl/ColdArmMist.part
		Part2	:LevitateHands.part
		Part3	:LevitateHandsMiddle.part
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part1	Powers/ColdControl/ColdArmMist.part
		Part2	:LevitateHands.part
		Part3	:LevitateHandsMiddle.part
	End

End


End				