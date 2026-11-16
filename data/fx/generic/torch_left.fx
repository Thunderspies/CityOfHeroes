
FxInfo

#####  torch prop   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Torch
		Type	Local
		At	WepL
		Geom	WepL_Torch_prop
	End
End

#####  Flame fx #############################################

Condition
	On 	Time  
	Time 	0

	Event
		Type	Local
		At	Torch
		Altpiv   1		
		Part1	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchflameA.part
		Part2	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchflameB.part
		Part3	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
		
	End

End


#####  Glow  ##########################################################

Condition
	On 	Time  
	Time 	0

	Event
		Type	Local
		At	Torch
		Altpiv   1
		Part1	Generic\\Torch_glow.part

		
	End

End




End		