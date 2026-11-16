#########################################################
##	template

FxInfo


Input
	Inpname WepR
End

Input
	Inpname WepL
End

Condition
	On Time 
	Time 0
	
	Event
		EName	Right
		Type 	Local
		At	WepR
		part1	:FireBallHead.part
		#Sound	Claw1 100.0 30.0 1.0
		
	End

	Event
		EName	Right
		Type 	Local
		At	WepL
		part1	:FireBallHead.part
		#Sound	Claw1 100.0 30.0 1.0
		
	End
	
End

	

End	

	