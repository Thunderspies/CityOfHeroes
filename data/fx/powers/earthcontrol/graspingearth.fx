#########################################################
##	Fire_Ball
##
FxInfo

Input  
	Inpname	WepL
End

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepL
		
		Part1	:EarthHands.part
		Part2	:EarthHandRocks.part
	End
	
End


Condition
	On	Time
	Time    25
		
	Event
		Ename	Prime
		Type	Destroy
	End
		
	
End

End	


			