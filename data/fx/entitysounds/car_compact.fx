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
		Sound silence_Loop 1 1 0
		Sound silence_Loop 1 1 0
		Sound silence_Loop 1 1 0
		Sound Beat5_Loop 150 150 1
		Sound Beat6_loop 150 150 .75
		Sound Beat7_loop 150 150 1
	End
End

Condition
On Time
Time 0
	Event
		Type	Local 
		At	Origin
		Sound CarCompact1_Loop 100 100 .7
	End
	Event
		Type	Local 
		At	Origin
		Sound CarCompact1Dist_Loop 200 200 .6
	End
End
End	



	