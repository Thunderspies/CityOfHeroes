#########################################################
##	
##

FxInfo


Input  
	InpName	Origin
End

##################################
Condition
	On 	Time
	Time 	1



	Event

		Type	Start
		At	Origin

		Part1	flash.part		
	End

End

##################################


Condition
	On 	Time
	Time 	130

	Event
		Ename 	All
		Type	Destroy
	End
End

End				