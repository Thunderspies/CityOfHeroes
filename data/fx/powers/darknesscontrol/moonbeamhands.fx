#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100

Input  
	Inpname	WepL
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End


Input  
	Inpname	WepR
End

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticleWhite.part
	
		Part3	:DarkHandsEmberb.part
		Sound darkwindup2 80 80 .7
		
		PMagnet	LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		#Part1	:DarkBlastTenticleWhite.part
	
		#Part3	:DarkHandsEmberb.part
				
		PMagnet	LarmL
	End

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticleWhite.part
		
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		#Part1	:DarkBlastTenticleWhite.part
		
		#Part3	:DarkHandsEmberb.part
		
		PMagnet	WepL
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName 	all
		Type	Destroy

	End

End


End	


			