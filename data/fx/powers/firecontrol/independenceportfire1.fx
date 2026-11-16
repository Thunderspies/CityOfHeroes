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
		Part1	:IndependencePort_flame_A1.part
		Part2	:IndependencePort_flame_B1.part
		Part4	:IndependencePortspark11.part
		Part2	:IndependencePort_flame_C1.part
		Sound torch2_loop 35 35 .33
	End


	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	:IndependencePortFlame011.part
		Sound Torch2_loop 50 50 .5
		
	End

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	:IndependencePortFlame021.part
			
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
		Part1	:IndependencePort_flame_Burst1.part
		LifeSpan	20
	End

End

End						