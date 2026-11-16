#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	root
End

LifeSpan	215

Condition
	On 	cycle
	Time 	68.999995
	Chance	1.0

	Event
		EName 	ring
		Type	local
		At	root
		bhvr	behaviors/lifespan2.bhvr
		Part1	:WilloWispFlame.part
		Part2	:WilloWispSpark.part	
		Part3	:WilloWispFlame01.part
		LifeSpan 46
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	local
		At	root
		Anim	FX_WilloWisps
		Sound flameburst_loop 100 100 .65
		Lifespan 208

	End

	Event
		EName 	FireChest
		Type	local
		At	Fire
		Animpiv	chest
		Part1	:FireHands2.part
		Lifespan 208

	End

	Event
		EName 	FireFace
		Type	local
		At	Fire
		Animpiv	Face
		Part1	:FireHands2.part
		Lifespan 208

	End

	Event
		EName 	FireNeck
		Type	local
		At	Fire
		Animpiv	Neck
		Part1	:FireHands2.part
		Lifespan 208

	End

	Event
		EName 	FireWaist
		Type	local
		At	Fire
		Animpiv	Waist
		Part1	:FireHands2.part
		Lifespan 208

	End

End

End
