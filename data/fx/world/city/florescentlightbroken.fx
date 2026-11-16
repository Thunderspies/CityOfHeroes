#########################################################
##	SStreetSteeam01
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
		Part1	world/City/FlorescentLightFlicker.part
		
	End
End


Condition
	On 	cycle
	Time 	10
	Chance	.8

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		
		Part2	world/City/FlorescentLightFlicker2.part
		Lifespan	10
	End
End

End			