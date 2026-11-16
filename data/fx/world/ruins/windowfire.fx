#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

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
		Part1	:WindowFireFlame_A.part
		Part2	:WindowFireFlame_B.part
		#Part4	:spark2.part
		Part5	:Window_Smoke.part
		Sound torch2_loop 37 37 .33
	End
End

End			