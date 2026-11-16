#########################################################
##	
##

FxInfo


Input  
	InpName	head
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Lifespan	150

Condition
	On 	Time
	Time 	30



	Event
		EName	Spike
		Type	Local
		At	head
		Geom	LocalPivot01
		Lifespan	80
	End

	Event
		EName	GasStream
		Type	Local
		At	spike
		Altpiv   4
		
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Part3	:TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
		
	End
	
End

End				