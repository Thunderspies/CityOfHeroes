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
		#Part1	:TFA.part
		Part2	:TFB.part
		
		Sound Torch1_loop 22.0 22.0 1.0
	End
End

End			