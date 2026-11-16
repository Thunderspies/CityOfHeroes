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
		Part1	WORLD/Lairs/cot/TorchflameA.part
		Part2	WORLD/Lairs/cot/TorchflameB.part
		Part3	WORLD/Lairs/cot/TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
		Sound Torch1a_loop 22.0 22.0 1.0
	End
End

End