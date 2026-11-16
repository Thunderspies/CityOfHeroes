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
		Part1	WORLD/Lairs/cot/Torchflame1NoTightenup.part
		Part2	WORLD/Lairs/cot/Torchflame2NoTightenup.part
		Part3	WORLD/Lairs/cot/Torchsparks.part
		Sound Torch1_loop 26.0 26.0 .85
	End
End

End			