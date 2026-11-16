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
		Part1	:IndependencePort_flame_A.part
		Part2	:IndependencePort_flame_B.part
		Part4	:IndependencePortspark1.part
		Part2	:IndependencePort_flame_C.part
		Sound torch2_loop 35 35 .33
	End


	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	:IndependencePortFlame01.part
		Sound Torch2_loop 50 50 .5
		
	End

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	:IndependencePortFlame02.part
			
	End


End

Condition
	On 	cycle
	Time 	63
	Chance	.4

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	:IndependencePort_flame_Burst.part
		LifeSpan	20
	End

End

End						