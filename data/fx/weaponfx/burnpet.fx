#########################################################
##	SStreetSteeam01
##
FxInfo
	
Input  
	InpName	root
End 

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	root
		Part1	:Mediumfire_flame_A.part
		Part2	:Mediumfire_flame_B.part
		Part4	:spark1.part
		Part2	:Mediumfire_flame_C.part
		Sound torch2_loop 35 35 .33
	End
End

End			