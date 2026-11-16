#########################################################
##	FireRing
##
FxInfo
	


Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	:IndependencePort_flame_A3.part
		Part2	:IndependencePort_flame_B3.part
		Part4	:IndependencePortspark13.part
		Part2	:IndependencePort_flame_C3.part
		Sound torch2_loop 35 35 .33
	End


	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	:IndependencePortFlame013.part
		Sound Torch2_loop 50 50 .5
		
	End

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	:IndependencePortFlame023.part
			
	End


End

Condition
	On 	cycle
	Time 	63
	Chance	.2

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	:IndependencePort_flame_Burst3.part
		LifeSpan	20
	End

End

End						