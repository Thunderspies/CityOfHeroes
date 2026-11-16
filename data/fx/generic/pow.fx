
#########################################################
##	iced target
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
		Part1	Generic/Generic_pow.part
		
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