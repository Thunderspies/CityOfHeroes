#########################################################
##	Michael Henry
##
FxInfo
Input  
	InpName	Origin
End
Condition
On Time
Time 0
	Event
		Type	Local 
		At	Origin
		Sound CarLux1_Loop 100 100 .9
	End
	Event
		Type	Local 
		At	Origin
		Sound CarLux1Dist_Loop 200 200 .7
	End
End

Condition
# On Time
# Time 0
	Event
		Type	Local 
		At	Origin
		Sound silence_Loop 1 1 0
		Sound silence_Loop 1 1 0
		Sound Beat1_Loop 150 150 .45
		Sound Beat2_Loop 150 150 .75
		Sound Beat3_Loop 150 150 1
		Sound Beat4_Loop 150 150 1
	End
End

End	



	