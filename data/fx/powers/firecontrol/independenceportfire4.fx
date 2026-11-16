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
		Part1	:IndependencePort_flame_A4.part
		Part2	:IndependencePort_flame_B4.part
		Part4	:IndependencePortspark14.part
		Part2	:IndependencePort_flame_C4.part
		Sound torch2_loop 35 35 .33
	End


	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	:IndependencePortFlame014.part
		Sound Torch2_loop 50 50 .5
		
	End

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	:IndependencePortFlame024.part
			
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
		Part1	:IndependencePort_flame_Burst4.part
		LifeSpan	20
	End

End

End						