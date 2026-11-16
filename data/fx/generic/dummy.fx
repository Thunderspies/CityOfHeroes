
#########################################################
##	Dummy effect for running the game with -nofx
##
FxInfo  

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		
	End

End


Condition
	On 	Time
	Time 	10

	Event
		EName 	All
		Type	Destroy 
	End
End


End			