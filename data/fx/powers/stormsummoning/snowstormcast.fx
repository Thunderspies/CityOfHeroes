#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Origin
End



##################################

Condition
	On	Time
	Time	0	

	Event
		Type	local
		At	Origin
		Sound coldblast2 100 100 .9
	End

End

Condition
	On	Time
	Time	10

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	powers/coldControl/ColdArmMist.part
		Part1	powers/coldControl/snowburst.part

		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	powers/coldControl/ColdArmMist.part
		Part1	powers/coldControl/snowburst.part

		LifeSpan	70

	End
End

Condition
	On	Time
	Time	20	

	Event
		Type	local
		At	Origin
		Sound cold_loop 100 100 .8
	End

End

