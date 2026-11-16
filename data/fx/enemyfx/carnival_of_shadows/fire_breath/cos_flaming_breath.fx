#########################################################
##	
##

FxInfo

Input 
	InpName Origin
End

Input
	InpName	Target
End

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
	Time 	0
	
	Event
		EName	Torch
		Type	Local
		At	WepR		
		GEOM	GEO_WepR_Staff_07	
	End

	Event
		EName	TorchFlame
		Type	Local
		At	Torch
		Altpiv   1		
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
		Part3	:TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
		
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Torchbreath
		Type	Local
		At	Torch
		Altpiv   1
		bhvr	behaviors/scale1a.bhvr		
	End

	Event
		Type	Posit
		At	Torchbreath
		Part1	:FireSpit.part
		Part2	:FireSpecksSpit.part
		Part3	:FireSpecksFalling.part
		Part4	:FireSpit2.part
		Part5	:FireSpitStart.part
		Magnet	Target
		LookAt	Target
		Sound arcoflame 100 100 .88
		Lifespan 50
		
	End
End

End				