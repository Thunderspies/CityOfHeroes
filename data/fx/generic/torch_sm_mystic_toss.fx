
FxInfo

##Lifespan	53

#####  torch prop   #######################################

Condition
	On 	Time  
	Time 	35

	Event
		EName	Torch					#existing event name, not totally clear
		Type	Local
		At	Mystic
		#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	WepR_torch_sm				#call to geometry fx
	End
End

#####  Flame fx #############################################

Condition
	On 	Time  
	Time 	36

	Event
		Type	Local
		At	Torch
		Bhvr	Behaviors/torch_sm.bhvr						#reference file to describe behavior, or in this case placement of torch flame node	
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
		Bhvr	Behaviors/torch_sm.bhvr
		Part1	Generic\\Torch_glow.part

		
	End

End




End		