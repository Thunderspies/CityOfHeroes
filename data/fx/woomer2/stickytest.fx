#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Posit 
		At	Root
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Part3	:TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
	End
End

End			