#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hook
		Type	Local 
		At	root
		Geom	RootToChestAdjustment
		Sound molotovhit1 80 80 .86
		Sound fireexplo 80 80 .72
		Sound firezap 80 80 .76
		Sound molotovhit1 80 80 .86
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	powers/firecontrol/FireBallTargetSteam.part
		Part2	powers/firecontrol/FireBallTargetFlame2.part
		Part3	powers/firecontrol/FireBallSpark.part	
		
		LifeSpan	50
			
	End


	Event
		EName 	Prime1
		Type	Local 
		At	root
		
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		

	End

	Event
		EName 	Prime2
		Type	Local 
		At	root
		Part1	powers/firecontrol/InfernoExplosion.part
		

	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		#Part1	powers/firecontrol/FireBallHitRing1.part
		#Part2	powers/firecontrol/FireBallHitRing2.part
		
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local 
		At	root
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset2.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset3.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	130
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset4.bhvr
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	130
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy 
	End


End

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			