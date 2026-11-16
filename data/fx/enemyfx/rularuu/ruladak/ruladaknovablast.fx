#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90


Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
		Sound Nova 100.0 100.0 .8
	End

End

Condition

	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	Local 
		At	root
		Bhvr	Behaviors/OffsetRuladakNova.bhvr
		
		Part2	:Inferno.part

	End

End

Condition

	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Local 
		At	root
		Bhvr	Behaviors/OffsetRuladakNova.bhvr
		
		Bhvr	Behaviors/CameraShake01.bhvr
		

	End

	Event
		EName 	Primex
		Type	Local 
		At	root
		Bhvr	Behaviors/OffsetRuladakNova.bhvr

		Part1	:InfernorockBits.part
		Part2	:InfernorockBits2.part
	End

End

Condition
	On 	Time
	Time 	60

	Event
		EName   RadiusFxScale
		Type	local
		At	Primex
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		
	End		


End

Condition
	On 	Time
	Time 	58
	Event
		EName 	Prime8
		Type	Local 
		At	root
		Bhvr	Behaviors/OffsetRuladakNova.bhvr
		Part   :Infernoflash.part
		Part   :InfernoflashA.part
		Part   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	62

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	FireRing
		bhvr	behaviors/FireRingScale.bhvr
		LifeSpan		1
	End

End

Condition
	On 	Time
	Time	80

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	87

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
