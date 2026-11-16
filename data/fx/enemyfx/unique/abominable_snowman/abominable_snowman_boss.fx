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
		Part1	:snowburst_Boss.part
		
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	Hips
		Part1	:snowburst_Boss.part

	End

		Event	
		Ename   RightHandMist
		Type	Local
		At	Head
		Part1	:snowburst_Boss.part

	End

End

Condition
	On	Time
	Time	0
		

	Event	
		Ename   LeftHandMist
		Type	Local
		At	HandR
		Part1	:ColdArmMist_Boss.part

	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	HandL
		Part1	:ColdArmMist_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	Hips
		Part1	:ColdArmMist_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	ULegR
		Part1	:ColdArmMist_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	ULegL
		Part1	:ColdArmMist_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	Head
		Part1	:ColdBodyMist_Boss.part

	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	CoL_R
		Part1	:ColdBodyMist_Boss.part
		Bhvr	:Col_R.Bhvr

	End

		Event	
		Ename   RightHandMist
		Type	Local
		At	CoL_L
		Part1	:ColdBodyMist_Boss.part
		Bhvr	:Col_L.Bhvr

	End
End


End				