#########################################################
##	Inferno FX
##
FxInfo
	
LifeSpan 40

Input  
	InpName	FootR
End



Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	Local 
		At	FootR
		Part1	powers/fireControl/InfernoExplosion.part
		Part2	powers/fireControl/Inferno.part

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	FootR
		Part1	powers/firecontrol/InfernorockBits.part		
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	FootR
		Bhvr	:Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part
		
	End		


End
Condition
	On 	Time
	Time 	5
	Event
		EName 	Prime8
		Type	Local 
		At	RadiusFxScale
		Part1   powers/firecontrol/Infernoflash.part

	End
End

Condition
	On 	Time
	Time	30

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End			